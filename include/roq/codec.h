/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>
#include <roq/logging.h>

#include <roq/libevent.h>

#include <cassert>

#include <limits>
#include <string>
#include <vector>

namespace roq {
namespace codec {

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
  size_t total_length;
};

const size_t HEADER_LENGTH = 21;
const uint8_t HEADER_MAGIC[] = { 0x20, 0x18 };
const uint8_t HEADER_FLAGS_FROM_CACHE = 0x01;
const uint8_t HEADER_FLAGS_SKIP = 0x02;

// buffer

class Buffer final {
 public:
  explicit Buffer(const size_t length = 65536) : _buffer(length) {}
  void reset() {
    _offset = 0;
  }
  message_t append(message_t message) {
    if (_buffer.size() <= (_offset + message.second))
      throw std::runtime_error("Buffer overflow");
    auto begin = &_buffer[_offset];
    std::memcpy(begin, message.first, message.second);
    message_t result { begin, message.second };
    _offset += message.second;
    return result;
  }

 private:
  Buffer(Buffer&) = delete;
  Buffer operator=(Buffer&) = delete;

 private:
  std::vector<uint8_t> _buffer;
  size_t _offset = 0;
};

// queue (will auto-reset the underlying buffer)

class Queue final {
 public:
  explicit Queue(Buffer& buffer) : _buffer(buffer) {}
  ~Queue() {
    _buffer.reset();
  }
  message_t push(message_t message) {
    auto result = _buffer.append(message);
    _messages.push_back(result);
    return result;
  }
  bool empty() const {
    return _messages.empty();
  }
  size_t size() const {
    return _messages.size();
  }
  const std::vector<message_t> get() const {
    return _messages;
  }
  void reset() {
    _messages.clear();
    _buffer.reset();
  }

 private:
  Queue(Queue&) = delete;
  Queue operator=(Queue&) = delete;

 private:
  Buffer& _buffer;
  std::vector<message_t> _messages;
};

// encode (events / low level)

inline flatbuffers::Offset<schema::SourceInfo>
convert(flatbuffers::FlatBufferBuilder& fbb, const SourceInfo& value) {
  return schema::CreateSourceInfo(
    fbb,
    value.seqno,
    time_point_to_uint64(value.create_time));
}

inline flatbuffers::Offset<schema::Handshake>
convert(flatbuffers::FlatBufferBuilder& fbb, const Handshake& value) {
  return schema::CreateHandshake(
    fbb,
    fbb.CreateString(value.api_version),
    fbb.CreateString(value.application),
    fbb.CreateString(value.hostname),
    value.pid,
    fbb.CreateString(value.uuid),
    fbb.CreateString(value.login),
    fbb.CreateString(value.password),
    fbb.CreateVectorOfStrings(value.subscriptions));
}

inline flatbuffers::Offset<schema::HandshakeAck>
convert(flatbuffers::FlatBufferBuilder& fbb, const HandshakeAck& value) {
  return schema::CreateHandshakeAck(
    fbb,
    fbb.CreateString(value.api_version),
    fbb.CreateString(value.application),
    fbb.CreateString(value.hostname),
    value.pid,
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
convert(flatbuffers::FlatBufferBuilder& fbb, const Heartbeat& value) {
  return schema::CreateHeartbeat(
    fbb,
    value.opaque);
}

inline flatbuffers::Offset<schema::HeartbeatAck>
convert(flatbuffers::FlatBufferBuilder& fbb, const HeartbeatAck& value) {
  return schema::CreateHeartbeatAck(
    fbb,
    value.opaque);
}

inline flatbuffers::Offset<schema::Ready>
convert(flatbuffers::FlatBufferBuilder& fbb, const Ready& value) {
  return schema::CreateReady(
    fbb,
    value.max_order_id);
}

inline flatbuffers::Offset<schema::GatewayStatus>
convert(flatbuffers::FlatBufferBuilder& fbb, const GatewayStatus& value) {
  return schema::CreateGatewayStatus(
    fbb,
    fbb.CreateString(value.name),
    value.status);
}

inline flatbuffers::Offset<schema::MarketByPrice>
convert(flatbuffers::FlatBufferBuilder& fbb, const MarketByPrice& value) {
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
convert(flatbuffers::FlatBufferBuilder& fbb, const TradeSummary& value) {
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
convert(flatbuffers::FlatBufferBuilder& fbb, const ReferenceData& value) {
  return schema::CreateReferenceData(
    fbb,
    fbb.CreateString(value.exchange),
    fbb.CreateString(value.instrument),
    value.tick_size,
    value.limit_up,
    value.limit_down);
}

inline flatbuffers::Offset<schema::MarketStatus>
convert(flatbuffers::FlatBufferBuilder& fbb, const MarketStatus& value) {
  return schema::CreateMarketStatus(
    fbb,
    fbb.CreateString(value.exchange),
    fbb.CreateString(value.instrument),
    value.trading_status);
}

inline flatbuffers::Offset<schema::CreateOrder>
convert(flatbuffers::FlatBufferBuilder& fbb, const CreateOrder& value) {
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
convert(flatbuffers::FlatBufferBuilder& fbb, const CreateOrderAck& value) {
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
convert(flatbuffers::FlatBufferBuilder& fbb, const ModifyOrder& value) {
  return schema::CreateModifyOrder(
    fbb,
    value.order_id,
    value.quantity_change,
    value.limit_price);
}

inline flatbuffers::Offset<schema::ModifyOrderAck>
convert(flatbuffers::FlatBufferBuilder& fbb, const ModifyOrderAck& value) {
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
convert(flatbuffers::FlatBufferBuilder& fbb, const CancelOrder& value) {
  return schema::CreateCancelOrder(
    fbb,
    value.order_id);
}

inline flatbuffers::Offset<schema::CancelOrderAck>
convert(flatbuffers::FlatBufferBuilder& fbb, const CancelOrderAck& value) {
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
convert(flatbuffers::FlatBufferBuilder& fbb, const OrderUpdate& value) {
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
convert(flatbuffers::FlatBufferBuilder& fbb, const TradeUpdate& value) {
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
convert(flatbuffers::FlatBufferBuilder& fbb, const PositionUpdate& value) {
  return schema::CreatePositionUpdate(
    fbb,
    fbb.CreateString(value.exchange),
    fbb.CreateString(value.instrument),
    value.trade_direction,
    value.position,
    value.position_yesterday,
    value.frozen_position,
    value.frozen_closing,
    value.frozen_closing_yesterday);
}

// encode (event)

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const SourceInfo& source_info,
    const Handshake& handshake) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::Handshake,
      convert(fbb, handshake).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const SourceInfo& source_info,
    const HandshakeAck& handshake_ack) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::HandshakeAck,
      convert(fbb, handshake_ack).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const SourceInfo& source_info,
    const Heartbeat& heartbeat) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::Heartbeat,
      convert(fbb, heartbeat).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const SourceInfo& source_info,
    const HeartbeatAck& heartbeat_ack) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::HeartbeatAck,
      convert(fbb, heartbeat_ack).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const SourceInfo& source_info,
    const Ready& ready) {
  return schema::CreateEvent(fbb,
      convert(fbb, source_info),
      schema::EventData::Ready,
      convert(fbb, ready).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const SourceInfo& source_info,
    const GatewayStatus& gateway_status) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::GatewayStatus,
      convert(fbb, gateway_status).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const SourceInfo& source_info,
    const ReferenceData& reference_data) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::ReferenceData,
      convert(fbb, reference_data).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const SourceInfo& source_info,
    const MarketStatus& market_status) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::MarketStatus,
      convert(fbb, market_status).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const SourceInfo& source_info,
    const MarketByPrice& market_by_price) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::MarketByPrice,
      convert(fbb, market_by_price).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const SourceInfo& source_info,
    const TradeSummary& trade_summary) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::TradeSummary,
      convert(fbb, trade_summary).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const SourceInfo& source_info,
    const CreateOrder& create_order) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::CreateOrder,
      convert(fbb, create_order).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const SourceInfo& source_info,
    const CreateOrderAck& create_order_ack) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::CreateOrderAck,
      convert(fbb, create_order_ack).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const SourceInfo& source_info,
    const ModifyOrder& modify_order) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::ModifyOrder,
      convert(fbb, modify_order).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const SourceInfo& source_info,
    const ModifyOrderAck& modify_order_ack) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::ModifyOrderAck,
      convert(fbb, modify_order_ack).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const SourceInfo& source_info,
    const CancelOrder& cancel_order) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::CancelOrder,
      convert(fbb, cancel_order).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const SourceInfo& source_info,
    const CancelOrderAck& cancel_order_ack) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::CancelOrderAck,
      convert(fbb, cancel_order_ack).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const SourceInfo& source_info,
    const OrderUpdate& order_update) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::OrderUpdate,
      convert(fbb, order_update).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const SourceInfo& source_info,
    const TradeUpdate& trade_update) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::TradeUpdate,
      convert(fbb, trade_update).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const SourceInfo& source_info,
    const PositionUpdate& position_update) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::PositionUpdate,
      convert(fbb, position_update).Union());
}

// encoder

class Encoder final {
 public:
  explicit Encoder(
      std::atomic<uint64_t>& seqno,
      flatbuffers::FlatBufferBuilder& fbb)
      : _seqno(seqno), _fbb(fbb) {}
  template <typename T>
  message_t encode(
      Queue& queue,
      const T& event) {
    return encode(queue, event, std::chrono::system_clock::now());
  }
  template <typename T>
  message_t encode(
      Queue& queue,
      const T& event,
      std::chrono::system_clock::time_point now) {
    SourceInfo source_info {
        .seqno = ++_seqno,  // TODO(thraneh): full barrier here, maybe acquire?
        .create_time = now,
    };
    _fbb.Clear();
    _fbb.Finish(convert2(_fbb, source_info, event));
    return queue.push(message_t { _fbb.GetBufferPointer(), _fbb.GetSize() });
  }

 private:
  Encoder(Encoder&) = delete;
  Encoder operator=(Encoder&) = delete;

 private:
  std::atomic<uint64_t>& _seqno;
  flatbuffers::FlatBufferBuilder& _fbb;
};

// writer

class Writer final {
 public:
  Writer() {}
  void write(libevent::Buffer& buffer, const Queue& queue, bool from_cache) {
    LOG_IF(FATAL, queue.empty()) << "Don't try to write if there's nothing to write!";
    ++_seqno;
    const auto& messages = queue.get();
    Header header = {};
    for (const auto& iter : messages)
      header.total_length += 2 + iter.second;
    if (std::numeric_limits<uint16_t>::max() < header.total_length)
      LOG(FATAL) << "Total length exceeds maximum";  // FIXME(thraneh): throw exception
    buffer.expand(HEADER_LENGTH + header.total_length);
    size_t bytes = 0;
    bytes += buffer.add(HEADER_MAGIC, sizeof(HEADER_MAGIC));
    uint16_t total_length = static_cast<uint16_t>(header.total_length);
    if (total_length == 0)
      LOG(FATAL) << "Total length can't be zero";  // FIXME(thraneh): throw exception
    bytes += buffer.add(&total_length, sizeof(total_length));
    uint8_t flags = (from_cache ? HEADER_FLAGS_FROM_CACHE : 0);
    bytes += buffer.add(&flags, sizeof(flags));
    bytes += buffer.add(&_seqno, sizeof(_seqno));
    uint64_t send_time = time_point_to_uint64(
        std::chrono::system_clock::now());
    bytes += buffer.add(&send_time, sizeof(send_time));
    assert(bytes == HEADER_LENGTH);
    for (const auto& iter : messages) {
      if (std::numeric_limits<uint16_t>::max() < iter.second)
        LOG(FATAL) << "Message length exceeds maximum";  // FIXME(thraneh): throw exception
      uint16_t length = static_cast<uint16_t>(iter.second);
      if (length == 0)
        LOG(FATAL) << "Message size can't be zero";  // FIXME(thraneh): throw exception
      bytes += buffer.add(&length, sizeof(length));
      bytes += buffer.add(iter.first, iter.second);
    }
    LOG_IF(FATAL, bytes != (HEADER_LENGTH + header.total_length)) << "Internal error!";
  }

 private:
  Writer(Writer&) = delete;
  Writer operator=(Writer&) = delete;

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
    auto receive_time = std::chrono::system_clock::now();
    while (true) {
      if (_header.total_length == 0) {  // header
        auto data = buffer.pullup(HEADER_LENGTH);
        if (data == nullptr)
          return messages;
        if (data[0] != HEADER_MAGIC[0] || data[1] != HEADER_MAGIC[1])
          LOG(FATAL) << "Incorrect header magic";  // FIXME(thraneh): throw exception
        uint16_t total_length;
        std::memcpy(&total_length, data + 2, 2);
        if (total_length == 0)
          LOG(FATAL) << "Total length can't be zero";  // FIXME(thraneh): throw exception
        bool from_cache = (data[4] & HEADER_FLAGS_FROM_CACHE) != 0;
        bool skip = (data[4] & HEADER_FLAGS_SKIP) != 0;
        uint64_t seqno, send_time;
        std::memcpy(&seqno, data + 5, 8);
        std::memcpy(&send_time, data + 13, 8);
        // assign
        _header.total_length = total_length;
        _batch_info.seqno = seqno;
        _batch_info.send_time = uint64_to_time_point(send_time);
        _batch_info.from_cache = from_cache;
        _receive_time = receive_time;
        _skip = skip;
        _remaining_bytes = total_length;
        _is_first = true;
        buffer.drain(HEADER_LENGTH);
      } else if (_skip) {
        auto data = buffer.pullup(_header.total_length);
        if (data == nullptr)
          return messages;
        buffer.drain(_header.total_length);
        std::memset(&_header, 0, sizeof(_header));
      } else if (_message_length == 0) {  // length(message)
        auto data = buffer.pullup(2);
        if (data == nullptr)
          return messages;
        LOG_IF(FATAL, _remaining_bytes < 2) << "Internal error " << _remaining_bytes ;
        _remaining_bytes -= 2;
        uint16_t message_length;
        std::memcpy(&message_length, data, 2);
        if (message_length == 0)
          LOG(FATAL) << "Message length can't be zero";  // FIXME(thraneh): throw exception
        _message_length = message_length;
        buffer.drain(2);
      } else {
        assert(_message_length > 0);
        auto data = buffer.pullup(_message_length);
        if (data == nullptr)
          return messages;
        LOG_IF(FATAL, _remaining_bytes < _message_length) << "Internal error " << _remaining_bytes << " " << _message_length;
        _remaining_bytes -= _message_length;
        auto is_last = _remaining_bytes == 0;
        _dispatcher.dispatch(
            data, _message_length, name.c_str(),
            _batch_info, _is_first, is_last,
            _receive_time);
        buffer.drain(_message_length);
        ++messages;
        _message_length = 0;
        _is_first = false;
        if (is_last) {
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
  bool _skip = false;
  size_t _message_length = 0;
  size_t _remaining_bytes = 0;
  bool _is_first = false;
};

// decode (events / low level)

inline SourceInfo convert(const schema::SourceInfo *value) {
  return SourceInfo {
    .seqno = value->seqno(),
    .create_time = uint64_to_time_point(value->create_time()),
  };
}

inline Handshake convert(const schema::Handshake *value) {
  Handshake result {
    .api_version = value->api_version()->c_str(),
    .application = value->application()->c_str(),
    .hostname = value->hostname()->c_str(),
    .pid = value->pid(),
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

inline HandshakeAck convert(const schema::HandshakeAck *value) {
  return HandshakeAck {
    .api_version = value->api_version()->c_str(),
    .application = value->application()->c_str(),
    .hostname = value->hostname()->c_str(),
    .pid = value->pid(),
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

inline Heartbeat convert(const schema::Heartbeat *value) {
  return Heartbeat {
    .opaque = value->opaque(),
  };
}

inline HeartbeatAck convert(const schema::HeartbeatAck *value) {
  return HeartbeatAck {
    .opaque = value->opaque(),
  };
}

inline Ready convert(const schema::Ready *value) {
  return Ready {
    .max_order_id = value->max_order_id(),
  };
}

inline GatewayStatus convert(const schema::GatewayStatus *value) {
  return GatewayStatus {
    .name = value->name()->c_str(),
    .status = value->status(),
  };
}

inline Layer convert(const schema::Layer *value) {
  return Layer {
    .bid_price = value->bid_price(),
    .bid_quantity = value->bid_quantity(),
    .ask_price = value->ask_price(),
    .ask_quantity = value->ask_quantity(),
  };
}

inline MarketByPrice convert(const schema::MarketByPrice *value) {
  MarketByPrice res {
    .exchange = value->exchange()->c_str(),
    .instrument = value->instrument()->c_str(),
  };
  const flatbuffers::Vector<const schema::Layer *> *depth = value->depth();
  assert(depth->Length() == MAX_DEPTH);
  for (auto i = 0; i < MAX_DEPTH; ++i) {
    res.depth[i] = convert((*depth)[i]);
  }
  res.exchange_time = uint64_to_time_point(value->exchange_time());
  res.channel = value->channel();
  return res;
}

inline TradeSummary convert(const schema::TradeSummary *value) {
  return TradeSummary {
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

inline ReferenceData convert(const schema::ReferenceData *value) {
  return ReferenceData {
    .exchange = value->exchange()->c_str(),
    .instrument = value->instrument()->c_str(),
    .tick_size = value->tick_size(),
    .limit_up = value->limit_up(),
    .limit_down = value->limit_down(),
  };
}

inline MarketStatus convert(const schema::MarketStatus *value) {
  return MarketStatus {
    .exchange = value->exchange()->c_str(),
    .instrument = value->instrument()->c_str(),
    .trading_status = value->trading_status(),
  };
}

inline CreateOrder convert(const schema::CreateOrder *value) {
  return CreateOrder {
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

inline CreateOrderAck convert(const schema::CreateOrderAck *value) {
  return CreateOrderAck {
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

inline ModifyOrder convert(const schema::ModifyOrder *value) {
  return ModifyOrder {
    .order_id = value->order_id(),
    .quantity_change = value->quantity_change(),
    .limit_price = value->limit_price(),
  };
}

inline ModifyOrderAck convert(const schema::ModifyOrderAck *value) {
  return ModifyOrderAck {
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

inline CancelOrder convert(const schema::CancelOrder *value) {
  return CancelOrder {
    .order_id = value->order_id(),
  };
}

inline CancelOrderAck convert(const schema::CancelOrderAck *value) {
  return CancelOrderAck {
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

inline OrderUpdate convert(const schema::OrderUpdate *value) {
  return OrderUpdate {
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

inline TradeUpdate convert(const schema::TradeUpdate *value) {
  return TradeUpdate {
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

inline PositionUpdate convert(const schema::PositionUpdate *value) {
  return PositionUpdate {
    .exchange = value->exchange()->c_str(),
    .instrument = value->instrument()->c_str(),
    .trade_direction = value->trade_direction(),
    .position = value->position(),
    .position_yesterday = value->position_yesterday(),
    .frozen_position = value->frozen_position(),
    .frozen_closing = value->frozen_closing(),
    .frozen_closing_yesterday = value->frozen_closing_yesterday(),
  };
}


// handler

class EventHandler {
 public:
  virtual void on(const BatchBeginEvent&) = 0;
  virtual void on(const BatchEndEvent&) = 0;
  virtual void on(const HandshakeEvent&) = 0;
  virtual void on(const HandshakeAckEvent&) = 0;
  virtual void on(const HeartbeatEvent&) = 0;
  virtual void on(const HeartbeatAckEvent&) = 0;
  virtual void on(const ReadyEvent&) = 0;
  virtual void on(const GatewayStatusEvent&) = 0;
  virtual void on(const ReferenceDataEvent&) = 0;
  virtual void on(const MarketStatusEvent&) = 0;
  virtual void on(const MarketByPriceEvent&) = 0;
  virtual void on(const TradeSummaryEvent&) = 0;
  virtual void on(const CreateOrderEvent&) = 0;
  virtual void on(const CreateOrderAckEvent&) = 0;
  virtual void on(const ModifyOrderEvent&) = 0;
  virtual void on(const ModifyOrderAckEvent&) = 0;
  virtual void on(const CancelOrderEvent&) = 0;
  virtual void on(const CancelOrderAckEvent&) = 0;
  virtual void on(const OrderUpdateEvent&) = 0;
  virtual void on(const TradeUpdateEvent&) = 0;
  virtual void on(const PositionUpdateEvent&) = 0;
};

// dispatch

class EventDispatcher final {
 public:
  explicit EventDispatcher(EventHandler& handler) : _handler(handler) {}
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
        .from_cache = batch_info.from_cache,
        .is_last = is_last,
        .channel = 0,
    };
    if (is_first)
      _handler.on(BatchBeginEvent { .message_info = message_info });
    auto type = item.event_data_type();
    switch (type) {
      case schema::EventData::Handshake: {
        auto handshake = convert(item.event_data_as_Handshake());
        HandshakeEvent event {
          .message_info = message_info,
          .handshake = handshake,
        };
        _handler.on(event);
        break;
      }
      case schema::EventData::HandshakeAck: {
        auto handshake_ack = convert(item.event_data_as_HandshakeAck());
        HandshakeAckEvent event {
          .message_info = message_info,
          .handshake_ack = handshake_ack,
        };
        _handler.on(event);
        break;
      }
      case schema::EventData::Heartbeat: {
        auto heartbeat = convert(item.event_data_as_Heartbeat());
        HeartbeatEvent event {
          .message_info = message_info,
          .heartbeat = heartbeat,
        };
        _handler.on(event);
        break;
      }
      case schema::EventData::HeartbeatAck: {
        auto heartbeat_ack = convert(item.event_data_as_HeartbeatAck());
        HeartbeatAckEvent event {
          .message_info = message_info,
          .heartbeat_ack = heartbeat_ack,
        };
        _handler.on(event);
        break;
      }
      case schema::EventData::Ready: {
        auto ready = convert(item.event_data_as_Ready());
        ReadyEvent event {
          .message_info = message_info,
          .ready = ready,
        };
        _handler.on(event);
        break;
      }
      case schema::EventData::GatewayStatus: {
        auto gateway_status = convert(item.event_data_as_GatewayStatus());
        GatewayStatusEvent event {
          .message_info = message_info,
          .gateway_status = gateway_status,
        };
        _handler.on(event);
        break;
      }
      case schema::EventData::ReferenceData: {
        auto reference_data = convert(item.event_data_as_ReferenceData());
        ReferenceDataEvent event {
          .message_info = message_info,
          .reference_data = reference_data,
        };
        _handler.on(event);
        break;
      }
      case schema::EventData::MarketStatus: {
        auto market_status = convert(item.event_data_as_MarketStatus());
        MarketStatusEvent event {
          .message_info = message_info,
          .market_status = market_status,
        };
        _handler.on(event);
        break;
      }
      case schema::EventData::MarketByPrice: {
        auto market_by_price = convert(item.event_data_as_MarketByPrice());
        MarketByPriceEvent event {
          .message_info = message_info,
          .market_by_price = market_by_price,
        };
        _handler.on(event);
        break;
      }
      case schema::EventData::TradeSummary: {
        auto trade_summary = convert(item.event_data_as_TradeSummary());
        TradeSummaryEvent event {
          .message_info = message_info,
          .trade_summary = trade_summary,
        };
        _handler.on(event);
        break;
      }
      case schema::EventData::CreateOrder: {
        auto create_order = convert(item.event_data_as_CreateOrder());
        CreateOrderEvent event {
          .message_info = message_info,
          .create_order = create_order,
        };
        _handler.on(event);
        break;
      }
      case schema::EventData::CreateOrderAck: {
        auto create_order_ack = convert(item.event_data_as_CreateOrderAck());
        CreateOrderAckEvent event {
          .message_info = message_info,
          .create_order_ack = create_order_ack,
        };
        _handler.on(event);
        break;
      }
      case schema::EventData::ModifyOrder: {
        auto modify_order = convert(item.event_data_as_ModifyOrder());
        ModifyOrderEvent event {
          .message_info = message_info,
          .modify_order = modify_order,
        };
        _handler.on(event);
        break;
      }
      case schema::EventData::ModifyOrderAck: {
        auto modify_order_ack = convert(item.event_data_as_ModifyOrderAck());
        ModifyOrderAckEvent event {
          .message_info = message_info,
          .modify_order_ack = modify_order_ack,
        };
        _handler.on(event);
        break;
      }
      case schema::EventData::CancelOrder: {
        auto cancel_order = convert(item.event_data_as_CancelOrder());
        CancelOrderEvent event {
          .message_info = message_info,
          .cancel_order = cancel_order,
        };
        _handler.on(event);
        break;
      }
      case schema::EventData::CancelOrderAck: {
        auto cancel_order_ack = convert(item.event_data_as_CancelOrderAck());
        CancelOrderAckEvent event {
          .message_info = message_info,
          .cancel_order_ack = cancel_order_ack};
        _handler.on(event);
        break;
      }
      case schema::EventData::OrderUpdate: {
        auto order_update = convert(item.event_data_as_OrderUpdate());
        OrderUpdateEvent event {
          .message_info = message_info,
          .order_update = order_update,
        };
        _handler.on(event);
        break;
      }
      case schema::EventData::TradeUpdate: {
        auto trade_update = convert(item.event_data_as_TradeUpdate());
        TradeUpdateEvent event {
          .message_info = message_info,
          .trade_update = trade_update,
        };
        _handler.on(event);
        break;
      }
      case schema::EventData::PositionUpdate: {
        auto position_update = convert(item.event_data_as_PositionUpdate());
        PositionUpdateEvent event {
          .message_info = message_info,
          .position_update = position_update,
        };
        _handler.on(event);
        break;
      }
      default: {
        LOG(FATAL) << "Unknown type=" << static_cast<int>(type);
      }
    }
    if (is_last)
      _handler.on(BatchEndEvent { .message_info = message_info });
  }

 private:
  EventDispatcher() = delete;
  EventDispatcher(EventDispatcher&) = delete;
  EventDispatcher& operator=(EventDispatcher&) = delete;

 private:
  EventHandler& _handler;
  flatbuffers::FlatBufferBuilder _flat_buffer_builder;
};

}  // namespace codec
}  // namespace roq