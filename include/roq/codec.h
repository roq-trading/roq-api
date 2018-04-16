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
  void operator=(Buffer&) = delete;

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
  void operator=(Queue&) = delete;

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

inline flatbuffers::Offset<schema::DownloadBegin>
convert(flatbuffers::FlatBufferBuilder& fbb, const DownloadBegin& value) {
  return schema::CreateDownloadBegin(
    fbb);
}

inline flatbuffers::Offset<schema::DownloadEnd>
convert(flatbuffers::FlatBufferBuilder& fbb, const DownloadEnd& value) {
  return schema::CreateDownloadEnd(
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
    fbb.CreateString(value.symbol),
    fbb.CreateVectorOfStructs(&depth[0], depth.size()),
    time_point_to_uint64(value.exchange_time),
    value.channel);
}

inline flatbuffers::Offset<schema::TradeSummary>
convert(flatbuffers::FlatBufferBuilder& fbb, const TradeSummary& value) {
  return schema::CreateTradeSummary(
    fbb,
    fbb.CreateString(value.exchange),
    fbb.CreateString(value.symbol),
    value.price,
    value.volume,
    value.turnover,
    value.side,
    time_point_to_uint64(value.exchange_time),
    value.channel);
}

inline flatbuffers::Offset<schema::ReferenceData>
convert(flatbuffers::FlatBufferBuilder& fbb, const ReferenceData& value) {
  return schema::CreateReferenceData(
    fbb,
    fbb.CreateString(value.exchange),
    fbb.CreateString(value.symbol),
    value.tick_size,
    value.limit_up,
    value.limit_down);
}

inline flatbuffers::Offset<schema::MarketStatus>
convert(flatbuffers::FlatBufferBuilder& fbb, const MarketStatus& value) {
  return schema::CreateMarketStatus(
    fbb,
    fbb.CreateString(value.exchange),
    fbb.CreateString(value.symbol),
    value.trading_status);
}

inline flatbuffers::Offset<schema::PositionUpdate>
convert(flatbuffers::FlatBufferBuilder& fbb, const PositionUpdate& value) {
  return schema::CreatePositionUpdate(
    fbb,
    fbb.CreateString(value.account),
    fbb.CreateString(value.exchange),
    fbb.CreateString(value.symbol),
    value.side,
    value.position,
    value.last_order_id);
}

inline flatbuffers::Offset<schema::OrderUpdate>
convert(flatbuffers::FlatBufferBuilder& fbb, const OrderUpdate& value) {
  return schema::CreateOrderUpdate(
    fbb,
    value.order_id,
    fbb.CreateString(value.account),
    fbb.CreateString(value.exchange),
    fbb.CreateString(value.symbol),
    value.order_status,
    value.side,
    value.remaining_quantity,
    value.traded_quantity,
    fbb.CreateString(value.order_template),
    time_point_to_uint64(value.insert_time),
    time_point_to_uint64(value.cancel_time),
    value.order_local_id,
    fbb.CreateString(value.order_external_id));
}

inline flatbuffers::Offset<schema::TradeUpdate>
convert(flatbuffers::FlatBufferBuilder& fbb, const TradeUpdate& value) {
  return schema::CreateTradeUpdate(
    fbb,
    value.trade_id,
    value.order_id,
    fbb.CreateString(value.account),
    fbb.CreateString(value.exchange),
    fbb.CreateString(value.symbol),
    value.side,
    value.quantity,
    value.price,
    fbb.CreateString(value.order_template),
    time_point_to_uint64(value.trade_time),
    value.order_local_id,
    fbb.CreateString(value.order_external_id),
    fbb.CreateString(value.trade_external_id));
}

inline flatbuffers::Offset<schema::CreateOrder>
convert(flatbuffers::FlatBufferBuilder& fbb, const CreateOrder& value) {
  return schema::CreateCreateOrder(
    fbb,
    value.order_id,
    fbb.CreateString(value.account),
    fbb.CreateString(value.exchange),
    fbb.CreateString(value.symbol),
    value.side,
    value.quantity,
    value.order_type,
    value.limit_price,
    value.time_in_force,
    fbb.CreateString(value.order_template));
}

inline flatbuffers::Offset<schema::ModifyOrder>
convert(flatbuffers::FlatBufferBuilder& fbb, const ModifyOrder& value) {
  return schema::CreateModifyOrder(
    fbb,
    value.order_id,
    value.quantity_change,
    value.limit_price);
}

inline flatbuffers::Offset<schema::CancelOrder>
convert(flatbuffers::FlatBufferBuilder& fbb, const CancelOrder& value) {
  return schema::CreateCancelOrder(
    fbb,
    value.order_id);
}

inline flatbuffers::Offset<schema::CreateOrderAck>
convert(flatbuffers::FlatBufferBuilder& fbb, const CreateOrderAck& value) {
  return schema::CreateCreateOrderAck(
    fbb,
    value.order_id,
    value.failure,
    fbb.CreateString(value.reason),
    value.order_local_id,
    fbb.CreateString(value.order_external_id));
}

inline flatbuffers::Offset<schema::ModifyOrderAck>
convert(flatbuffers::FlatBufferBuilder& fbb, const ModifyOrderAck& value) {
  return schema::CreateModifyOrderAck(
    fbb,
    value.order_id,
    value.failure,
    fbb.CreateString(value.reason),
    value.order_local_id,
    fbb.CreateString(value.order_external_id));
}

inline flatbuffers::Offset<schema::CancelOrderAck>
convert(flatbuffers::FlatBufferBuilder& fbb, const CancelOrderAck& value) {
  return schema::CreateCancelOrderAck(
    fbb,
    value.order_id,
    value.failure,
    fbb.CreateString(value.reason),
    value.order_local_id,
    fbb.CreateString(value.order_external_id));
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
    const DownloadBegin& download_begin) {
  return schema::CreateEvent(fbb,
      convert(fbb, source_info),
      schema::EventData::DownloadBegin,
      convert(fbb, download_begin).Union());
}

inline flatbuffers::Offset<schema::Event> convert2(
    flatbuffers::FlatBufferBuilder& fbb,
    const SourceInfo& source_info,
    const DownloadEnd& download_end) {
  return schema::CreateEvent(fbb,
      convert(fbb, source_info),
      schema::EventData::DownloadEnd,
      convert(fbb, download_end).Union());
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
    const PositionUpdate& position_update) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::PositionUpdate,
      convert(fbb, position_update).Union());
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
    const CancelOrderAck& cancel_order_ack) {
  return schema::CreateEvent(
      fbb,
      convert(fbb, source_info),
      schema::EventData::CancelOrderAck,
      convert(fbb, cancel_order_ack).Union());
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
  void operator=(Encoder&) = delete;

 private:
  std::atomic<uint64_t>& _seqno;
  flatbuffers::FlatBufferBuilder& _fbb;
};

// writer

class Writer final {
 public:
  Writer() {}
  void write(libevent::Buffer& buffer, const Queue& queue, bool from_cache) {
    // payload length + validate
    const auto& messages = queue.get();
    size_t payload_length = 0;
    for (const auto& iter : messages) {
      LOG_IF(FATAL, iter.second == 0) << "Message length can't be zero";
      LOG_IF(FATAL, std::numeric_limits<uint16_t>::max() < iter.second) <<
          "Message length exceeds maximum";
      payload_length += 2 + iter.second;
    }
    // header
    write_header(buffer, from_cache, payload_length);
    // payload
    size_t bytes = 0;
    for (const auto& iter : messages) {
      uint16_t length = static_cast<uint16_t>(iter.second);
      bytes += buffer.add(&length, sizeof(length));
      bytes += buffer.add(iter.first, iter.second);
    }
    LOG_IF(FATAL, bytes != payload_length) << "Internal error";
  }
  void write(libevent::Buffer& buffer, const message_t& payload, bool from_cache) {
    write_header(buffer, from_cache, payload.second);
    buffer.add(payload.first, payload.second);
  }

 private:
  void write_header(libevent::Buffer& buffer, bool from_cache, size_t payload_length) {
    // validate
    LOG_IF(FATAL, payload_length == 0) << "Payload length can't be zero";
    LOG_IF(FATAL, std::numeric_limits<uint16_t>::max() < payload_length) <<
        "Total length exceeds maximum";
    // fields
    uint16_t length = static_cast<uint16_t>(payload_length);
    uint8_t flags = (from_cache ? HEADER_FLAGS_FROM_CACHE : 0);
    uint64_t send_time = time_point_to_uint64(
        std::chrono::system_clock::now());
    ++_seqno;
    // reserve buffer space
    buffer.expand(HEADER_LENGTH + payload_length);
    // write
    size_t bytes = 0;
    bytes += buffer.add(HEADER_MAGIC, sizeof(HEADER_MAGIC));
    bytes += buffer.add(&length, sizeof(length));
    bytes += buffer.add(&flags, sizeof(flags));
    bytes += buffer.add(&_seqno, sizeof(_seqno));
    bytes += buffer.add(&send_time, sizeof(send_time));
    LOG_IF(FATAL, bytes != HEADER_LENGTH) << "Internal error";
  }

 private:
  Writer(Writer&) = delete;
  void operator=(Writer&) = delete;

 private:
  uint64_t _seqno = 0;
};

// decoder

template <typename Dispatcher>
class Decoder final {
 public:
  explicit Decoder(Dispatcher& dispatcher) : _dispatcher(dispatcher) {}
  void reset() {
    _payload_length = 0;  // not necessary to reset any other fields
  }
  size_t dispatch(libevent::Buffer& buffer, const std::string& name) {
    size_t messages = 0;
    // note!
    // receive time recorded when we're first told data has become available
    // try to avoid measuring the time spent on internal dispatching
    auto receive_time = std::chrono::system_clock::now();
    while (true) {
      if (_payload_length == 0) {  // header
        auto data = buffer.pullup(HEADER_LENGTH);
        if (data == nullptr)
          return messages;
        // validate
        LOG_IF(FATAL, data[0] != HEADER_MAGIC[0] ||
                      data[1] != HEADER_MAGIC[1]) << "Internal error";
        uint16_t payload_length;
        std::memcpy(&payload_length, data + 2, 2);
        uint8_t flags = data[4];
        bool from_cache = (flags & HEADER_FLAGS_FROM_CACHE) != 0;
        bool skip = (flags & HEADER_FLAGS_SKIP) != 0;
        uint64_t seqno;
        std::memcpy(&seqno, data + 5, 8);
        uint64_t send_time;
        std::memcpy(&send_time, data + 13, 8);
        // validate
        LOG_IF(FATAL, payload_length == 0) << "Internal error";
        // assign
        _payload_length = payload_length;
        _batch_info.seqno = seqno;
        _batch_info.send_time = uint64_to_time_point(send_time);
        _batch_info.from_cache = from_cache;
        _receive_time = receive_time;
        _skip = skip;
        _is_first = true;
        // drain from buffer
        buffer.drain(HEADER_LENGTH);
      } else if (_skip) {
        auto data = buffer.pullup(_payload_length);
        if (data == nullptr)
          return messages;
        buffer.drain(_payload_length);
        _payload_length = 0;
      } else if (_message_length == 0) {  // length(message)
        auto data = buffer.pullup(2);
        if (data == nullptr)
          return messages;
        LOG_IF(FATAL, _payload_length < 2) << "Internal error";
        _payload_length -= 2;
        uint16_t message_length;
        std::memcpy(&message_length, data, 2);
        LOG_IF(FATAL, message_length == 0) << "Internal error";
        _message_length = message_length;
        buffer.drain(2);
      } else {
        LOG_IF(FATAL, _message_length == 0) << "Internal error";
        auto data = buffer.pullup(_message_length);
        if (data == nullptr)
          return messages;
        LOG_IF(FATAL, _payload_length < _message_length) << "Internal error";
        _payload_length -= _message_length;
        auto is_last = _payload_length == 0;
        _dispatcher.dispatch(
            data,
            _message_length,
            name.c_str(),
            _batch_info,
            _is_first,
            is_last,
            _receive_time);
        buffer.drain(_message_length);
        ++messages;
        _message_length = 0;
        _is_first = false;
        if (is_last)
          _payload_length = 0;
      }
    }
  }

 private:
  Decoder(Decoder&) = delete;
  void operator=(Decoder&) = delete;

 private:
  Dispatcher& _dispatcher;
  uint16_t _payload_length = 0;
  BatchInfo _batch_info = {};
  time_point_t _receive_time;
  bool _skip = false;
  size_t _message_length = 0;
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

inline DownloadBegin convert(const schema::DownloadBegin *value) {
  return DownloadBegin {
  };
}

inline DownloadEnd convert(const schema::DownloadEnd *value) {
  return DownloadEnd {
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
    .symbol = value->symbol()->c_str(),
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
    .symbol = value->symbol()->c_str(),
    .price = value->price(),
    .volume = value->volume(),
    .turnover = value->turnover(),
    .side = value->side(),
    .exchange_time = uint64_to_time_point(value->exchange_time()),
    .channel = value->channel(),
  };
}

inline ReferenceData convert(const schema::ReferenceData *value) {
  return ReferenceData {
    .exchange = value->exchange()->c_str(),
    .symbol = value->symbol()->c_str(),
    .tick_size = value->tick_size(),
    .limit_up = value->limit_up(),
    .limit_down = value->limit_down(),
  };
}

inline MarketStatus convert(const schema::MarketStatus *value) {
  return MarketStatus {
    .exchange = value->exchange()->c_str(),
    .symbol = value->symbol()->c_str(),
    .trading_status = value->trading_status(),
  };
}

inline PositionUpdate convert(const schema::PositionUpdate *value) {
  return PositionUpdate {
    .account = value->account()->c_str(),
    .exchange = value->exchange()->c_str(),
    .symbol = value->symbol()->c_str(),
    .side = value->side(),
    .position = value->position(),
    .last_order_id = value->last_order_id(),
  };
}
inline OrderUpdate convert(const schema::OrderUpdate *value) {
  return OrderUpdate {
    .order_id = value->order_id(),
    .account = value->account()->c_str(),
    .exchange = value->exchange()->c_str(),
    .symbol = value->symbol()->c_str(),
    .order_status = value->order_status(),
    .side = value->side(),
    .remaining_quantity = value->remaining_quantity(),
    .traded_quantity = value->traded_quantity(),
    .order_template = value->order_template()->c_str(),
    .insert_time = uint64_to_time_point(value->insert_time()),
    .cancel_time = uint64_to_time_point(value->cancel_time()),
    .order_local_id = value->order_local_id(),
    .order_external_id = value->order_external_id()->c_str(),
  };
}

inline TradeUpdate convert(const schema::TradeUpdate *value) {
  return TradeUpdate {
    .trade_id = value->trade_id(),
    .order_id = value->order_id(),
    .account = value->account()->c_str(),
    .exchange = value->exchange()->c_str(),
    .symbol = value->symbol()->c_str(),
    .side = value->side(),
    .quantity = value->quantity(),
    .price = value->price(),
    .order_template = value->order_template()->c_str(),
    .trade_time = uint64_to_time_point(value->trade_time()),
    .order_local_id = value->order_local_id(),
    .order_external_id = value->order_external_id()->c_str(),
    .trade_external_id = value->trade_external_id()->c_str(),
  };
}

inline CreateOrder convert(const schema::CreateOrder *value) {
  return CreateOrder {
    .order_id = value->order_id(),
    .account = value->account()->c_str(),
    .exchange = value->exchange()->c_str(),
    .symbol = value->symbol()->c_str(),
    .side = value->side(),
    .quantity = value->quantity(),
    .order_type = value->order_type(),
    .limit_price = value->limit_price(),
    .time_in_force = value->time_in_force(),
    .order_template = value->order_template()->c_str(),
  };
}

inline ModifyOrder convert(const schema::ModifyOrder *value) {
  return ModifyOrder {
    .order_id = value->order_id(),
    .quantity_change = value->quantity_change(),
    .limit_price = value->limit_price(),
  };
}

inline CancelOrder convert(const schema::CancelOrder *value) {
  return CancelOrder {
    .order_id = value->order_id(),
  };
}

inline CreateOrderAck convert(const schema::CreateOrderAck *value) {
  return CreateOrderAck {
    .order_id = value->order_id(),
    .failure = value->failure(),
    .reason = value->reason()->c_str(),
    .order_local_id = value->order_local_id(),
    .order_external_id = value->order_external_id()->c_str(),
  };
}

inline ModifyOrderAck convert(const schema::ModifyOrderAck *value) {
  return ModifyOrderAck {
    .order_id = value->order_id(),
    .failure = value->failure(),
    .reason = value->reason()->c_str(),
    .order_local_id = value->order_local_id(),
    .order_external_id = value->order_external_id()->c_str(),
  };
}

inline CancelOrderAck convert(const schema::CancelOrderAck *value) {
  return CancelOrderAck {
    .order_id = value->order_id(),
    .failure = value->failure(),
    .reason = value->reason()->c_str(),
    .order_local_id = value->order_local_id(),
    .order_external_id = value->order_external_id()->c_str(),
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
  virtual void on(const DownloadBeginEvent&) = 0;
  virtual void on(const DownloadEndEvent&) = 0;
  virtual void on(const GatewayStatusEvent&) = 0;
  virtual void on(const MarketByPriceEvent&) = 0;
  virtual void on(const TradeSummaryEvent&) = 0;
  virtual void on(const ReferenceDataEvent&) = 0;
  virtual void on(const MarketStatusEvent&) = 0;
  virtual void on(const PositionUpdateEvent&) = 0;
  virtual void on(const OrderUpdateEvent&) = 0;
  virtual void on(const TradeUpdateEvent&) = 0;
  virtual void on(const CreateOrderEvent&) = 0;
  virtual void on(const ModifyOrderEvent&) = 0;
  virtual void on(const CancelOrderEvent&) = 0;
  virtual void on(const CreateOrderAckEvent&) = 0;
  virtual void on(const ModifyOrderAckEvent&) = 0;
  virtual void on(const CancelOrderAckEvent&) = 0;
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
      case schema::EventData::DownloadBegin: {
        auto download_begin = convert(item.event_data_as_DownloadBegin());
        DownloadBeginEvent event {
          .message_info = message_info,
          .download_begin = download_begin,
        };
        _handler.on(event);
        break;
      }
      case schema::EventData::DownloadEnd: {
        auto download_end = convert(item.event_data_as_DownloadEnd());
        DownloadEndEvent event {
          .message_info = message_info,
          .download_end = download_end,
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
      case schema::EventData::PositionUpdate: {
        auto position_update = convert(item.event_data_as_PositionUpdate());
        PositionUpdateEvent event {
          .message_info = message_info,
          .position_update = position_update,
        };
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
      case schema::EventData::CreateOrder: {
        auto create_order = convert(item.event_data_as_CreateOrder());
        CreateOrderEvent event {
          .message_info = message_info,
          .create_order = create_order,
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
      case schema::EventData::CancelOrder: {
        auto cancel_order = convert(item.event_data_as_CancelOrder());
        CancelOrderEvent event {
          .message_info = message_info,
          .cancel_order = cancel_order,
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
      case schema::EventData::ModifyOrderAck: {
        auto modify_order_ack = convert(item.event_data_as_ModifyOrderAck());
        ModifyOrderAckEvent event {
          .message_info = message_info,
          .modify_order_ack = modify_order_ack,
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
      default: {
        LOG(FATAL) << "Unknown type=" << static_cast<int>(type);
      }
    }
    if (is_last)
      _handler.on(BatchEndEvent { .message_info = message_info });
  }

 private:
  EventDispatcher(EventDispatcher&) = delete;
  void operator=(EventDispatcher&) = delete;

 private:
  EventHandler& _handler;
  flatbuffers::FlatBufferBuilder _flat_buffer_builder;
};

}  // namespace codec
}  // namespace roq
