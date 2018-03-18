/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <quinclas/tradingapi.h>
#include <quinclas/logging.h>

#include <quinclas/libevent.h>

#include <cassert>

#include <limits>
#include <string>
#include <vector>

namespace quinclas {
namespace common {


// time_point utilities

inline uint64_t time_point_to_uint64(time_point_t time_point) {
  auto microseconds =
    std::chrono::time_point_cast<std::chrono::microseconds>(time_point);
  return microseconds.time_since_epoch().count();
}

inline time_point_t uint64_to_time_point(uint64_t microseconds) {
  auto duration = std::chrono::duration_cast<duration_t>(
      std::chrono::microseconds(microseconds));
  return time_point_t(duration);
}

// header

struct Header final {
  size_t message_count;
  size_t total_length;
};

const size_t HEADER_LENGTH = 22;
const uint8_t HEADER_MAGIC[] = { 0x20, 0x18 };
const uint8_t HEADER_FLAGS_IS_CACHED = 0x01;

// queue

class Queue final {
 public:
  explicit Queue(const size_t length) : _buffer(length) {}
  void reset() {
    _offset = 0;
  }
  message_t append(const void* data, const size_t length) {
    if (_buffer.size() <= (_offset + length))
      throw std::runtime_error("Buffer overflow");
    auto begin = &_buffer[_offset];
    std::memcpy(begin, data, length);
    _offset += length;
    return message_t { begin, length };
  }
 private:
  std::vector<uint8_t> _buffer;
  size_t _offset = 0;
};

// encoder

class Encoder final {
 public:
  explicit Encoder(Queue& queue) : _queue(queue) {}
  // HANS -- also externally provided time !!!
  template <typename T>
  message_t encode(const T& event) {
    SourceInfo source_info {
        .seqno = ++_seqno,
        .create_time = std::chrono::system_clock::now(),
    };
    _fbb.Clear();
    _fbb.Finish(convert2(_fbb, source_info, event));
    return _queue.append(_fbb.GetBufferPointer(), _fbb.GetSize());
  }

 private:
  Queue& _queue;
  flatbuffers::FlatBufferBuilder _fbb;
  uint64_t _seqno = 0;
};

// writer

class Writer final {
 public:
  void write(
      libevent::Buffer& buffer,
      const std::vector<message_t>& messages,
      bool is_cached) {
    ++_seqno;
    Header header = { .message_count = messages.size() };
    for (const auto& iter : messages)
      header.total_length += 2 + iter.second;
    if (std::numeric_limits<uint8_t>::max() < header.message_count)
      std::abort();  // FIXME(thraneh): throw exception
    if (std::numeric_limits<uint16_t>::max() < header.total_length)
      std::abort();  // FIXME(thraneh): throw exception
    buffer.expand(HEADER_LENGTH + header.total_length);
    size_t bytes = 0;
    bytes += buffer.add(HEADER_MAGIC, sizeof(HEADER_MAGIC));
    uint16_t total_length = static_cast<uint16_t>(header.total_length);
    bytes += buffer.add(&total_length, sizeof(total_length));
    uint8_t message_count = static_cast<uint8_t>(header.message_count);
    bytes += buffer.add(&message_count, sizeof(message_count));
    uint8_t flags = (is_cached ? HEADER_FLAGS_IS_CACHED : 0);
    bytes += buffer.add(&flags, sizeof(flags));
    bytes += buffer.add(&_seqno, sizeof(_seqno));
    uint64_t send_time = time_point_to_uint64(
        std::chrono::system_clock::now());
    bytes += buffer.add(&send_time, sizeof(send_time));
    assert(bytes == HEADER_LENGTH);
    for (const auto& iter : messages) {
      if (std::numeric_limits<uint16_t>::max() < iter.second)
        std::abort();  // FIXME(thraneh): throw exception
      uint16_t length = static_cast<uint16_t>(iter.second);
      bytes += buffer.add(&length, sizeof(length));
      bytes += buffer.add(iter.first, iter.second);
    }
    assert(bytes == (HEADER_LENGTH + header.total_length));
  }

 private:
  uint64_t _seqno = 0;
};

// decoder

template <typename Dispatcher>
class Decoder final {
 public:
  explicit Decoder(Dispatcher& dispatcher) : _dispatcher(dispatcher) {}
  void reset() {
    std::memset(&_header, 0, sizeof(_header));
    std::memset(&_batch_info, 0, sizeof(_batch_info));
    _message_length = 0;
    _is_first = false;
  }
  size_t dispatch(libevent::Buffer& buffer, const std::string& name) {
    size_t messages = 0;
    while (true) {
      if (_header.total_length == 0) {  // header
        auto data = buffer.pullup(HEADER_LENGTH);
        if (data == nullptr)
          return messages;
        if (data[0] != HEADER_MAGIC[0] || data[1] != HEADER_MAGIC[1])
          std::abort();  // FIXME(thraneh): throw exception
        uint16_t total_length;
        std::memcpy(&total_length, data + 2, 2);
        if (total_length == 0)
          std::abort();  // FIXME(thraneh): throw exception
        uint8_t message_count = data[4];
        bool is_cached = (data[5] & HEADER_FLAGS_IS_CACHED) != 0;
        uint64_t seqno, send_time;
        std::memcpy(&seqno, data + 6, 8);
        std::memcpy(&send_time, data + 14, 8);
        // assign
        _header.total_length = total_length;
        _header.message_count = message_count;
        _batch_info.seqno = seqno;
        _batch_info.send_time = uint64_to_time_point(send_time);
        _batch_info.is_cached = is_cached;
        _receive_time = std::chrono::system_clock::now();
        _is_first = true;
        buffer.drain(HEADER_LENGTH);
      } else if (_header.message_count == 0) {  // skip-frame
        auto data = buffer.pullup(_header.total_length);
        if (data == nullptr)
          return messages;
        buffer.drain(_header.total_length);
        std::memset(&_header, 0, sizeof(_header));
      } else if (_message_length == 0) {  // length(message)
        auto data = buffer.pullup(2);
        if (data == nullptr)
          return messages;
        uint16_t message_length;
        std::memcpy(&message_length, data, 2);
        if (message_length == 0)
          std::abort();  // FIXME(thraneh): throw exception
        _message_length = message_length;
        buffer.drain(2);
      } else {
        assert(_message_length > 0);
        auto data = buffer.pullup(_message_length);
        if (data == nullptr)
          return messages;
        auto is_last = (_header.message_count == 1);
        _dispatcher.dispatch(
            data, _message_length, name.c_str(),
            _batch_info, _is_first, is_last,
            _receive_time);
        buffer.drain(_message_length);
        ++messages;
        _message_length = 0;
        _is_first = false;
        if (0 == --_header.message_count) {
          std::memset(&_header, 0, sizeof(_header));
          _receive_time = {};
        }
      }
    }
  }

 private:
  Decoder(Decoder&) = delete;
  Decoder& operator=(Decoder&) = delete;

 private:
  Dispatcher& _dispatcher;
  Header _header = {};
  BatchInfo _batch_info = {};
  time_point_t _receive_time;
  size_t _message_length = 0;
  bool _is_first = false;
};

// decode (events / low level)

inline common::SourceInfo convert(const schema::SourceInfo *value) {
  return common::SourceInfo {
    .seqno = value->seqno(),
    .create_time = uint64_to_time_point(value->create_time()),
  };
}

inline common::Handshake convert(const schema::Handshake *value) {
  common::Handshake result {
    .api_version = value->api_version()->c_str(),
    .uuid = value->uuid()->c_str(),
    .login = value->login()->c_str(),
    .password = value->password()->c_str(),
  };
  const flatbuffers::Vector<flatbuffers::Offset<flatbuffers::String> > *subscriptions = value->subscriptions();
  auto length = subscriptions->Length();
  result.subscriptions.reserve(length);
  for (auto i = 0; i < length; ++i) {
    result.subscriptions.push_back((*subscriptions)[i]->c_str());
  }
  return result;
}

inline common::HandshakeAck convert(const schema::HandshakeAck *value) {
  return common::HandshakeAck {
    .failure = value->failure(),
    .failure_reason = value->failure_reason()->c_str(),
    .server_uuid = value->server_uuid()->c_str(),
    .server_name = value->server_name()->c_str(),
    .shmem_s2c_name = value->shmem_s2c_name()->c_str(),
    .shmem_s2c_size = value->shmem_s2c_size(),
    .shmem_c2s_name = value->shmem_c2s_name()->c_str(),
    .shmem_c2s_size = value->shmem_c2s_size(),
  };
}

inline common::Heartbeat convert(const schema::Heartbeat *value) {
  return common::Heartbeat {
    .opaque = uint64_to_time_point(value->opaque()),
  };
}

inline common::HeartbeatAck convert(const schema::HeartbeatAck *value) {
  return common::HeartbeatAck {
    .opaque = uint64_to_time_point(value->opaque()),
  };
}

inline common::Ready convert(const schema::Ready *value) {
  return common::Ready {};
}

inline common::GatewayStatus convert(const schema::GatewayStatus *value) {
  return common::GatewayStatus {
    .market_data = value->market_data(),
    .order_management = value->order_management(),
  };
}

inline common::Layer convert(const schema::Layer *value) {
  return common::Layer {
    .bid_price = value->bid_price(),
    .bid_quantity = value->bid_quantity(),
    .ask_price = value->ask_price(),
    .ask_quantity = value->ask_quantity(),
  };
}

inline common::MarketByPrice convert(const schema::MarketByPrice *value) {
  common::MarketByPrice res {
    .exchange = value->exchange()->c_str(),
    .instrument = value->instrument()->c_str(),
  };
  const flatbuffers::Vector<const schema::Layer *> *depth = value->depth();
  assert(depth->Length() == common::MAX_DEPTH);
  for (auto i = 0; i < common::MAX_DEPTH; ++i) {
    res.depth[i] = convert((*depth)[i]);
  }
  res.exchange_time = uint64_to_time_point(value->exchange_time());
  res.channel = value->channel();
  return res;
}

inline common::TradeSummary convert(const schema::TradeSummary *value) {
  return common::TradeSummary {
    .exchange = value->exchange()->c_str(),
    .instrument = value->instrument()->c_str(),
    .price = value->price(),
    .volume = value->volume(),
    .turnover = value->turnover(),
    .direction = value->direction(),
    .exchange_time = uint64_to_time_point(value->exchange_time()),
    .channel = value->channel(),
  };
}

inline common::ReferenceData convert(const schema::ReferenceData *value) {
  return common::ReferenceData {
    .exchange = value->exchange()->c_str(),
    .instrument = value->instrument()->c_str(),
    .tick_size = value->tick_size(),
    .limit_up = value->limit_up(),
    .limit_down = value->limit_down(),
  };
}

inline common::MarketStatus convert(const schema::MarketStatus *value) {
  return common::MarketStatus {
    .exchange = value->exchange()->c_str(),
    .instrument = value->instrument()->c_str(),
    .trading_status = value->trading_status(),
  };
}

inline common::CreateOrder convert(const schema::CreateOrder *value) {
  return common::CreateOrder {
    .opaque = value->opaque(),
    .order_template = value->order_template()->c_str(),
    .exchange = value->exchange()->c_str(),
    .instrument = value->instrument()->c_str(),
    .direction = value->direction(),
    .quantity = value->quantity(),
    .limit_price = value->limit_price(),
    .stop_price = value->stop_price(),
  };
}

inline common::CreateOrderAck convert(const schema::CreateOrderAck *value) {
  return common::CreateOrderAck {
    .opaque = value->opaque(),
    .order_id = value->order_id(),
    .failure = value->failure(),
    .reason = value->reason()->c_str(),
    .order_template = value->order_template()->c_str(),
    .external_order_id = value->external_order_id()->c_str(),
    .exchange = value->exchange()->c_str(),
    .instrument = value->instrument()->c_str(),
  };
}

inline common::ModifyOrder convert(const schema::ModifyOrder *value) {
  return common::ModifyOrder {
    .order_id = value->order_id(),
    .quantity_change = value->quantity_change(),
    .limit_price = value->limit_price(),
  };
}

inline common::ModifyOrderAck convert(const schema::ModifyOrderAck *value) {
  return common::ModifyOrderAck {
    .opaque = value->opaque(),
    .order_id = value->order_id(),
    .failure = value->failure(),
    .reason = value->reason()->c_str(),
    .order_template = value->order_template()->c_str(),
    .external_order_id = value->external_order_id()->c_str(),
    .exchange = value->exchange()->c_str(),
    .instrument = value->instrument()->c_str(),
    .quantity_change = value->quantity_change(),
    .limit_price = value->limit_price(),
  };
}

inline common::CancelOrder convert(const schema::CancelOrder *value) {
  return common::CancelOrder {
    .order_id = value->order_id(),
  };
}

inline common::CancelOrderAck convert(const schema::CancelOrderAck *value) {
  return common::CancelOrderAck {
    .opaque = value->opaque(),
    .order_id = value->order_id(),
    .failure = value->failure(),
    .reason = value->reason()->c_str(),
    .order_template = value->order_template()->c_str(),
    .external_order_id = value->external_order_id()->c_str(),
    .exchange = value->exchange()->c_str(),
    .instrument = value->instrument()->c_str(),
  };
}

inline common::OrderUpdate convert(const schema::OrderUpdate *value) {
  return common::OrderUpdate {
    .opaque = value->opaque(),
    .order_id = value->order_id(),
    .order_template = value->order_template()->c_str(),
    .external_order_id = value->external_order_id()->c_str(),
    .exchange = value->exchange()->c_str(),
    .instrument = value->instrument()->c_str(),
    .status = value->status(),
    .trade_direction = value->trade_direction(),
    .remaining_quantity = value->remaining_quantity(),
    .traded_quantity = value->traded_quantity(),
    .insert_time = uint64_to_time_point(value->insert_time()),
    .cancel_time = uint64_to_time_point(value->cancel_time()),
  };
}

inline common::TradeUpdate convert(const schema::TradeUpdate *value) {
  return common::TradeUpdate {
    .order_id = value->order_id(),
    .external_order_id = value->external_order_id()->c_str(),
    .external_trade_id = value->external_trade_id()->c_str(),
    .exchange = value->exchange()->c_str(),
    .instrument = value->instrument()->c_str(),
    .trade_direction = value->trade_direction(),
    .quantity = value->quantity(),
    .price = value->price(),
    .trade_time = uint64_to_time_point(value->trade_time()),
  };
}

inline common::PositionUpdate convert(const schema::PositionUpdate *value) {
  return common::PositionUpdate {
    .exchange = value->exchange()->c_str(),
    .instrument = value->instrument()->c_str(),
    .position = value->position(),
  };
}


// encode (events / low level)

inline flatbuffers::Offset<schema::SourceInfo>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::SourceInfo& value) {
  return schema::CreateSourceInfo(
    fbb,
    value.seqno,
    time_point_to_uint64(value.create_time));
}

inline flatbuffers::Offset<schema::Handshake>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::Handshake& value) {
  return schema::CreateHandshake(
    fbb,
    fbb.CreateString(value.api_version),
    fbb.CreateString(value.uuid),
    fbb.CreateString(value.login),
    fbb.CreateString(value.password),
    fbb.CreateVectorOfStrings(value.subscriptions));
}

inline flatbuffers::Offset<schema::HandshakeAck>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::HandshakeAck& value) {
  return schema::CreateHandshakeAck(
    fbb,
    value.failure,
    fbb.CreateString(value.failure_reason),
    fbb.CreateString(value.server_uuid),
    fbb.CreateString(value.server_name),
    fbb.CreateString(value.shmem_s2c_name),
    value.shmem_s2c_size,
    fbb.CreateString(value.shmem_c2s_name),
    value.shmem_c2s_size);
}

inline flatbuffers::Offset<schema::Heartbeat>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::Heartbeat& value) {
  return schema::CreateHeartbeat(
    fbb,
    time_point_to_uint64(value.opaque));
}

inline flatbuffers::Offset<schema::HeartbeatAck>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::HeartbeatAck& value) {
  return schema::CreateHeartbeatAck(
    fbb,
    time_point_to_uint64(value.opaque));
}

inline flatbuffers::Offset<schema::Ready>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::Ready& value) {
  return schema::CreateReady(fbb);
}

inline flatbuffers::Offset<schema::GatewayStatus>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::GatewayStatus& value) {
  return schema::CreateGatewayStatus(
    fbb,
    value.market_data,
    value.order_management);
}

inline flatbuffers::Offset<schema::MarketByPrice>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::MarketByPrice& value) {
  std::vector<schema::Layer> depth(MAX_DEPTH);
  for (auto i = 0; i < MAX_DEPTH; ++i)
    depth[i] = schema::Layer(
      value.depth[i].bid_price,
      value.depth[i].bid_quantity,
      value.depth[i].ask_price,
      value.depth[i].ask_quantity);
  return schema::CreateMarketByPrice(
    fbb,
    fbb.CreateString(value.exchange),
    fbb.CreateString(value.instrument),
    fbb.CreateVectorOfStructs(&depth[0], depth.size()),
    time_point_to_uint64(value.exchange_time),
    value.channel);
}

inline flatbuffers::Offset<schema::TradeSummary>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::TradeSummary& value) {
  return schema::CreateTradeSummary(
    fbb,
    fbb.CreateString(value.exchange),
    fbb.CreateString(value.instrument),
    value.price,
    value.volume,
    value.turnover,
    value.direction,
    time_point_to_uint64(value.exchange_time),
    value.channel);
}

inline flatbuffers::Offset<schema::ReferenceData>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::ReferenceData& value) {
  return schema::CreateReferenceData(
    fbb,
    fbb.CreateString(value.exchange),
    fbb.CreateString(value.instrument),
    value.tick_size,
    value.limit_up,
    value.limit_down);
}

inline flatbuffers::Offset<schema::MarketStatus>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::MarketStatus& value) {
  return schema::CreateMarketStatus(
    fbb,
    fbb.CreateString(value.exchange),
    fbb.CreateString(value.instrument),
    value.trading_status);
}

inline flatbuffers::Offset<schema::CreateOrder>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::CreateOrder& value) {
  return schema::CreateCreateOrder(
    fbb,
    value.opaque,
    fbb.CreateString(value.order_template),
    fbb.CreateString(value.exchange),
    fbb.CreateString(value.instrument),
    value.direction,
    value.quantity,
    value.limit_price,
    value.stop_price);
}

inline flatbuffers::Offset<schema::CreateOrderAck>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::CreateOrderAck& value) {
  return schema::CreateCreateOrderAck(
    fbb,
    value.opaque,
    value.order_id,
    value.failure,
    fbb.CreateString(value.reason),
    fbb.CreateString(value.order_template),
    fbb.CreateString(value.external_order_id),
    fbb.CreateString(value.exchange),
    fbb.CreateString(value.instrument));
}

inline flatbuffers::Offset<schema::ModifyOrder>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::ModifyOrder& value) {
  return schema::CreateModifyOrder(
    fbb,
    value.order_id,
    value.quantity_change,
    value.limit_price);
}

inline flatbuffers::Offset<schema::ModifyOrderAck>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::ModifyOrderAck& value) {
  return schema::CreateModifyOrderAck(
    fbb,
    value.opaque,
    value.order_id,
    value.failure,
    fbb.CreateString(value.reason),
    fbb.CreateString(value.order_template),
    fbb.CreateString(value.external_order_id),
    fbb.CreateString(value.exchange),
    fbb.CreateString(value.instrument),
    value.quantity_change,
    value.limit_price);
}

inline flatbuffers::Offset<schema::CancelOrder>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::CancelOrder& value) {
  return schema::CreateCancelOrder(
    fbb,
    value.order_id);
}

inline flatbuffers::Offset<schema::CancelOrderAck>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::CancelOrderAck& value) {
  return schema::CreateCancelOrderAck(
    fbb,
    value.opaque,
    value.order_id,
    value.failure,
    fbb.CreateString(value.reason),
    fbb.CreateString(value.order_template),
    fbb.CreateString(value.external_order_id),
    fbb.CreateString(value.exchange),
    fbb.CreateString(value.instrument));
}

inline flatbuffers::Offset<schema::OrderUpdate>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::OrderUpdate& value) {
  return schema::CreateOrderUpdate(
    fbb,
    value.opaque,
    value.order_id,
    fbb.CreateString(value.order_template),
    fbb.CreateString(value.external_order_id),
    fbb.CreateString(value.exchange),
    fbb.CreateString(value.instrument),
    value.status,
    value.trade_direction,
    value.remaining_quantity,
    value.traded_quantity,
    time_point_to_uint64(value.insert_time),
    time_point_to_uint64(value.cancel_time));
}

inline flatbuffers::Offset<schema::TradeUpdate>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::TradeUpdate& value) {
  return schema::CreateTradeUpdate(
    fbb,
    value.order_id,
    fbb.CreateString(value.external_order_id),
    fbb.CreateString(value.external_trade_id),
    fbb.CreateString(value.exchange),
    fbb.CreateString(value.instrument),
    value.trade_direction,
    value.quantity,
    value.price,
    time_point_to_uint64(value.trade_time));
}

inline flatbuffers::Offset<schema::PositionUpdate>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::PositionUpdate& value) {
  return schema::CreatePositionUpdate(
    fbb,
    fbb.CreateString(value.exchange),
    fbb.CreateString(value.instrument),
    value.position);
}


// encode (event2)

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const common::SourceInfo& source_info,
    const common::Handshake& handshake) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::Handshake,
      convert(fbb, handshake).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const common::SourceInfo& source_info,
    const common::HandshakeAck& handshake_ack) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::HandshakeAck,
      convert(fbb, handshake_ack).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const common::SourceInfo& source_info,
    const common::Heartbeat& heartbeat) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::Heartbeat,
      convert(fbb, heartbeat).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const common::SourceInfo& source_info,
    const common::HeartbeatAck& heartbeat_ack) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::HeartbeatAck,
      convert(fbb, heartbeat_ack).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const common::SourceInfo& source_info,
    const common::Ready& ready) {
  return schema::CreateEvent(fbb,
      convert(fbb, source_info),
      schema::EventData::Ready,
      convert(fbb, ready).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const common::SourceInfo& source_info,
    const common::GatewayStatus& gateway_status) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::GatewayStatus,
      convert(fbb, gateway_status).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const common::SourceInfo& source_info,
    const common::ReferenceData& reference_data) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::ReferenceData,
      convert(fbb, reference_data).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const common::SourceInfo& source_info,
    const common::MarketStatus& market_status) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::MarketStatus,
      convert(fbb, market_status).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const common::SourceInfo& source_info,
    const common::MarketByPrice& market_by_price) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::MarketByPrice,
      convert(fbb, market_by_price).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const common::SourceInfo& source_info,
    const common::TradeSummary& trade_summary) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::TradeSummary,
      convert(fbb, trade_summary).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const common::SourceInfo& source_info,
    const common::CreateOrder& create_order) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::CreateOrder,
      convert(fbb, create_order).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const common::SourceInfo& source_info,
    const common::CreateOrderAck& create_order_ack) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::CreateOrderAck,
      convert(fbb, create_order_ack).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const common::SourceInfo& source_info,
    const common::ModifyOrder& modify_order) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::ModifyOrder,
      convert(fbb, modify_order).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const common::SourceInfo& source_info,
    const common::ModifyOrderAck& modify_order_ack) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::ModifyOrderAck,
      convert(fbb, modify_order_ack).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const common::SourceInfo& source_info,
    const common::CancelOrder& cancel_order) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::CancelOrder,
      convert(fbb, cancel_order).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const common::SourceInfo& source_info,
    const common::CancelOrderAck& cancel_order_ack) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::CancelOrderAck,
      convert(fbb, cancel_order_ack).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const common::SourceInfo& source_info,
    const common::OrderUpdate& order_update) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::OrderUpdate,
      convert(fbb, order_update).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const common::SourceInfo& source_info,
    const common::TradeUpdate& trade_update) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::TradeUpdate,
      convert(fbb, trade_update).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const common::SourceInfo& source_info,
    const common::PositionUpdate& position_update) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::PositionUpdate,
      convert(fbb, position_update).Union());
}

// dispatch

// HANS -- make this generic -- let the user decide on filtering
class ClientEventDispatcher final {
 public:
  ClientEventDispatcher(Client& client, Strategy& strategy)
      : _client(client), _strategy(strategy) {}
  void dispatch(
      const void *buffer, const size_t length,
      const char *source, const BatchInfo& batch_info,
      bool is_first, bool is_last, time_point_t receive_time) {
    auto root = flatbuffers::GetRoot<schema::Event>(buffer);  // security issue: length is not being validated
    const auto& item = *root;
    auto source_info = convert(item.source_info());
    MessageInfo message_info {
        .source = source,
        .source_create_time = source_info.create_time,
        .client_receive_time = receive_time,
        .routing_latency = receive_time - batch_info.send_time,
        .is_cached = batch_info.is_cached,
        .is_last = is_last,
        .channel = 0
    };
    if (is_first)
      _strategy.on(BatchBeginEvent { .message_info = message_info });
    auto type = item.event_data_type();
    switch (type) {
      case schema::EventData::Handshake: {
        LOG(WARNING) << "Unexpected Handshake";
        break;
      }
      case schema::EventData::HandshakeAck: {
        auto handshake_ack = convert(item.event_data_as_HandshakeAck());
        HandshakeAckEvent event {
          .message_info = message_info,
          .handshake_ack = handshake_ack
        };
        VLOG(1) << "HandshakeAckEvent " << event;
        _client.on(event);
        break;
      }
      case schema::EventData::Heartbeat: {
        LOG(WARNING) << "Unexpected Heartbeat";
        break;
      }
      case schema::EventData::HeartbeatAck: {
        auto heartbeat_ack = convert(item.event_data_as_HeartbeatAck());
        HeartbeatAckEvent event {
          .message_info = message_info,
          .heartbeat_ack = heartbeat_ack};
        VLOG(1) << "HeartbeatAckEvent " << event;
        _client.on(event);
        break;
      }
      case schema::EventData::Ready: {
        auto ready = convert(item.event_data_as_Ready());
        ReadyEvent event {
          .message_info = message_info,
          .ready = ready};
        VLOG(1) << "ReadyEvent " << event;
        _strategy.on(event);
        break;
      }
      case schema::EventData::GatewayStatus: {
        auto gateway_status = convert(item.event_data_as_GatewayStatus());
        GatewayStatusEvent event {
          .message_info = message_info,
          .gateway_status = gateway_status};
        VLOG(1) << "GatewayStatusEvent " << event;
        _strategy.on(event);
        break;
      }
      case schema::EventData::ReferenceData: {
        auto reference_data = convert(item.event_data_as_ReferenceData());
        ReferenceDataEvent event {
          .message_info = message_info,
          .reference_data = reference_data};
        VLOG(1) << "ReferenceDataEvent " << event;
        _strategy.on(event);
        break;
      }
      case schema::EventData::MarketStatus: {
        auto market_status = convert(item.event_data_as_MarketStatus());
        MarketStatusEvent event {
          .message_info = message_info,
          .market_status = market_status};
        VLOG(1) << "MarketStatusEvent " << event;
        _strategy.on(event);
        break;
      }
      case schema::EventData::MarketByPrice: {
        auto market_by_price = convert(item.event_data_as_MarketByPrice());
        MarketByPriceEvent event {
          .message_info = message_info,
          .market_by_price = market_by_price};
        VLOG(1) << "MarketByPriceEvent " << event;
        _strategy.on(event);
        break;
      }
      case schema::EventData::TradeSummary: {
        auto trade_summary = convert(item.event_data_as_TradeSummary());
        TradeSummaryEvent event {
          .message_info = message_info,
          .trade_summary = trade_summary};
        VLOG(1) << "TradeSummaryEvent " << event;
        _strategy.on(event);
        break;
      }
      case schema::EventData::CreateOrder: {
        LOG(WARNING) << "Unexpected CreateOrder";
        break;
      }
      case schema::EventData::CreateOrderAck: {
        auto create_order_ack = convert(item.event_data_as_CreateOrderAck());
        CreateOrderAckEvent event {
          .message_info = message_info,
          .create_order_ack = create_order_ack};
        VLOG(1) << "CreateOrderAck " << event;
        _strategy.on(event);
        break;
      }
      case schema::EventData::ModifyOrder: {
        LOG(WARNING) << "Unexpected ModifyOrder";
        break;
      }
      case schema::EventData::ModifyOrderAck: {
        auto modify_order_ack = convert(item.event_data_as_ModifyOrderAck());
        ModifyOrderAckEvent event {
          .message_info = message_info,
          .modify_order_ack = modify_order_ack};
        VLOG(1) << "ModifyOrderAck " << event;
        _strategy.on(event);
        break;
      }
      case schema::EventData::CancelOrder: {
        LOG(WARNING) << "Unexpected CancelOrder";
        break;
      }
      case schema::EventData::CancelOrderAck: {
        auto cancel_order_ack = convert(item.event_data_as_CancelOrderAck());
        CancelOrderAckEvent event {
          .message_info = message_info,
          .cancel_order_ack = cancel_order_ack};
        VLOG(1) << "CancelOrderAck " << event;
        _strategy.on(event);
        break;
      }
      case schema::EventData::OrderUpdate: {
        auto order_update = convert(item.event_data_as_OrderUpdate());
        OrderUpdateEvent event {
          .message_info = message_info,
          .order_update = order_update};
        VLOG(1) << "OrderUpdateEvent " << event;
        _strategy.on(event);
        break;
      }
      case schema::EventData::TradeUpdate: {
        auto trade_update = convert(item.event_data_as_TradeUpdate());
        TradeUpdateEvent event {
          .message_info = message_info,
          .trade_update = trade_update};
        VLOG(1) << "TradeUpdateEvent " << event;
        _strategy.on(event);
        break;
      }
      case schema::EventData::PositionUpdate: {
        auto position_update = convert(item.event_data_as_PositionUpdate());
        PositionUpdateEvent event {
          .message_info = message_info,
          .position_update = position_update};
        VLOG(1) << "TradeUpdateEvent " << event;
        _strategy.on(event);
        break;
      }
      default: {
        LOG(FATAL) << "Unknown type=" << static_cast<int>(type);
      }
    }
    if (is_last)
      _strategy.on(BatchEndEvent { .message_info = message_info });
  }

 private:
  ClientEventDispatcher() = delete;
  ClientEventDispatcher(ClientEventDispatcher&) = delete;
  ClientEventDispatcher& operator=(ClientEventDispatcher&) = delete;

 private:
  Client& _client;
  Strategy& _strategy;
  flatbuffers::FlatBufferBuilder _flat_buffer_builder;
};

}  // namespace common
}  // namespace quinclas
