/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <quinclas/tradingapi.h>

#include <cassert>

#include <limits>
#include <vector>

namespace quinclas {
namespace common {

// envelope

struct Envelope final {
  static const size_t LENGTH = 8;
  static size_t decode(const void *buffer, uint32_t *latency = nullptr) {
    const auto buffer_ = reinterpret_cast<const uint8_t *>(buffer);
    if (buffer_[0] != 0x20 || buffer_[1] != 0x17)
      throw std::runtime_error("Envelope is corrupted");
    if (latency != nullptr) {
      auto time_point = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
      uint32_t now = static_cast<uint32_t>(time_point.time_since_epoch().count());
      uint32_t sent;
      memcpy(&sent, &buffer_[4], sizeof(sent));
      *latency = (sent <= now) ? (now - sent) : std::numeric_limits<uint32_t>::max() - ((sent - 1) - now);
    }
    return (static_cast<int>(buffer_[2]) << 8) + static_cast<int>(buffer_[3]);
  }
  static void encode(void *buffer, const size_t length) {
    // TODO(thraneh): validate length
    auto buffer_ = reinterpret_cast<uint8_t *>(buffer);
    buffer_[0] = 0x20;
    buffer_[1] = 0x17;
    buffer_[2] = static_cast<uint8_t>(static_cast<int>(length) >> 8);
    buffer_[3] = static_cast<uint8_t>(static_cast<int>(length));
    auto time_point = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
    uint32_t now = static_cast<uint32_t>(time_point.time_since_epoch().count());
    memcpy(&buffer_[4], &now, sizeof(now));
  }
};

// time_point utilities

inline time_point_t uint64_to_time_point(uint64_t microseconds) {
  auto duration = std::chrono::microseconds(microseconds);
  return time_point_t(duration);
}

inline uint64_t time_point_to_uint64(time_point_t time_point) {
  auto microseconds = std::chrono::time_point_cast<std::chrono::microseconds>(time_point);
  return microseconds.time_since_epoch().count();
}

// decode (events / low level)

inline common::MessageInfo convert(const schema::MessageInfo *value) {
  return common::MessageInfo{
    .gateway = value->gateway()->c_str(),
    .message_id = value->message_id(),
    .exchange_time = uint64_to_time_point(value->exchange_time()),
    .receive_time = uint64_to_time_point(value->receive_time()),
    .enqueue_time = uint64_to_time_point(value->enqueue_time()),
  };
}

inline common::HandshakeAck convert(const schema::HandshakeAck *value) {
  return common::HandshakeAck{
    .api_version = value->api_version()->c_str(),
    .failure = value->failure(),
    .reason = value->reason()->c_str(),
  };
}

inline common::HeartbeatAck convert(const schema::HeartbeatAck *value) {
  return common::HeartbeatAck{
    .opaque = uint64_to_time_point(value->opaque()),
  };
}

inline common::GatewayStatus convert(const schema::GatewayStatus *value) {
  return common::GatewayStatus{
    .market_data_connection_status = value->market_data_connection_status(),
    .market_data_login_status = value->market_data_login_status(),
    .order_management_connection_status = value->order_management_connection_status(),
    .order_management_login_status = value->order_management_login_status(),
  };
}

inline common::ReferenceData convert(const schema::ReferenceData *value) {
  return common::ReferenceData{
    .exchange = value->exchange()->c_str(),
    .instrument = value->instrument()->c_str(),
    .tick_size = value->tick_size(),
    .limit_up = value->limit_up(),
    .limit_down = value->limit_down(),
  };
}

inline common::MarketStatus convert(const schema::MarketStatus *value) {
  return common::MarketStatus{
    .exchange = value->exchange()->c_str(),
    .instrument = value->instrument()->c_str(),
    .trading_status = value->trading_status(),
  };
}

inline common::Layer convert(const schema::Layer *value) {
  return common::Layer{
    .bid_price = value->bid_price(),
    .bid_quantity = value->bid_quantity(),
    .ask_price = value->ask_price(),
    .ask_quantity = value->ask_quantity(),
  };
}

inline common::MarketByPrice convert(const schema::MarketByPrice *value) {
  common::MarketByPrice res = {
    .exchange = value->exchange()->c_str(),
    .instrument = value->instrument()->c_str(),
  };
  const flatbuffers::Vector<const schema::Layer *> *depth = value->depth();
  assert(depth->Length() == common::MAX_DEPTH);
  for (auto i = 0; i < common::MAX_DEPTH; ++i) {
    res.depth[i] = convert((*depth)[i]);
  }
  return res;
}

inline common::TradeSummary convert(const schema::TradeSummary *value) {
  return common::TradeSummary{
    .exchange = value->exchange()->c_str(),
    .instrument = value->instrument()->c_str(),
    .price = value->price(),
    .volume = value->volume(),
    .turnover = value->turnover(),
    .direction = value->direction(),
  };
}

inline common::CreateOrderAck convert(const schema::CreateOrderAck *value) {
  return common::CreateOrderAck{
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

inline common::ModifyOrderAck convert(const schema::ModifyOrderAck *value) {
  return common::ModifyOrderAck{
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

inline common::CancelOrderAck convert(const schema::CancelOrderAck *value) {
  return common::CancelOrderAck{
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
  return common::OrderUpdate{
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
  return common::TradeUpdate{
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

// decode (requests / low level)

inline common::RequestInfo convert(const schema::RequestInfo *value) {
  return common::RequestInfo{
    .destination = value->destination()->c_str(),
    .trace_source = value->trace_source()->c_str(),
    .trace_message_id = value->trace_message_id(),
    .send_time = uint64_to_time_point(value->send_time()),
    .receive_time = uint64_to_time_point(value->receive_time()),
  };
}

inline common::Handshake convert(const schema::Handshake *value) {
  return common::Handshake{
    .api_version = value->api_version()->c_str(),
    .login = value->login()->c_str(),
    .password = value->password()->c_str(),
  };
}

inline common::Heartbeat convert(const schema::Heartbeat *value) {
  return common::Heartbeat{
    .opaque = uint64_to_time_point(value->opaque()),
  };
}

inline common::CreateOrder convert(const schema::CreateOrder *value) {
  return common::CreateOrder{
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

inline common::ModifyOrder convert(const schema::ModifyOrder *value) {
  return common::ModifyOrder{
    .order_id = value->order_id(),
    .quantity_change = value->quantity_change(),
    .limit_price = value->limit_price(),
  };
}

inline common::CancelOrder convert(const schema::CancelOrder *value) {
  return common::CancelOrder{
    .order_id = value->order_id(),
  };
}

// encode (events / low level)

inline flatbuffers::Offset<schema::MessageInfo>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::MessageInfo& value) {
  return schema::CreateMessageInfo(
    fbb,
    fbb.CreateString(value.gateway),
    value.message_id,
    time_point_to_uint64(value.exchange_time),
    time_point_to_uint64(value.receive_time),
    time_point_to_uint64(value.enqueue_time));
}

inline flatbuffers::Offset<schema::HandshakeAck>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::HandshakeAck& value) {
  return schema::CreateHandshakeAck(
    fbb,
    fbb.CreateString(value.api_version),
    value.failure,
    fbb.CreateString(value.reason));
}

inline flatbuffers::Offset<schema::HeartbeatAck>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::HeartbeatAck& value) {
  return schema::CreateHeartbeatAck(
    fbb,
    time_point_to_uint64(value.opaque));
}

inline flatbuffers::Offset<schema::GatewayStatus>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::GatewayStatus& value) {
  return schema::CreateGatewayStatus(
    fbb,
    value.market_data_connection_status,
    value.market_data_login_status,
    value.order_management_connection_status,
    value.order_management_login_status);
}

inline flatbuffers::Offset<schema::MarketByPrice>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::MarketByPrice& value) {
  std::vector<schema::Layer> depth(MAX_DEPTH);  // TODO(thraneh): recycle this array?
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
    fbb.CreateVectorOfStructs(&depth[0], depth.size()));
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
    value.direction);
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

// encode (events)

inline flatbuffers::Offset<schema::Event>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::HandshakeAckEvent& value) {
  auto message_info = convert(fbb, value.message_info);
  auto handshake_ack = convert(fbb, value.handshake_ack);
  return schema::CreateEvent(fbb, message_info, schema::EventData::HandshakeAck, handshake_ack.Union());
}

inline flatbuffers::Offset<schema::Event>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::HeartbeatAckEvent& value) {
  auto message_info = convert(fbb, value.message_info);
  auto heartbeat_ack = convert(fbb, value.heartbeat_ack);
  return schema::CreateEvent(fbb, message_info, schema::EventData::HeartbeatAck, heartbeat_ack.Union());
}

inline flatbuffers::Offset<schema::Event>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::GatewayStatusEvent& value) {
  auto message_info = convert(fbb, value.message_info);
  auto gateway_status = convert(fbb, value.gateway_status);
  return schema::CreateEvent(fbb, message_info, schema::EventData::GatewayStatus, gateway_status.Union());
}

inline flatbuffers::Offset<schema::Event>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::ReferenceDataEvent& value) {
  auto message_info = convert(fbb, value.message_info);
  auto reference_data = convert(fbb, value.reference_data);
  return schema::CreateEvent(fbb, message_info, schema::EventData::ReferenceData, reference_data.Union());
}

inline flatbuffers::Offset<schema::Event>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::MarketStatusEvent& value) {
  auto message_info = convert(fbb, value.message_info);
  auto market_status = convert(fbb, value.market_status);
  return schema::CreateEvent(fbb, message_info, schema::EventData::MarketStatus, market_status.Union());
}

inline flatbuffers::Offset<schema::Event>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::MarketByPriceEvent& value) {
  auto message_info = convert(fbb, value.message_info);
  auto market_by_price = convert(fbb, value.market_by_price);
  return schema::CreateEvent(fbb, message_info, schema::EventData::MarketByPrice, market_by_price.Union());
}

inline flatbuffers::Offset<schema::Event>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::TradeSummaryEvent& value) {
  auto message_info = convert(fbb, value.message_info);
  auto trade_summary = convert(fbb, value.trade_summary);
  return schema::CreateEvent(fbb, message_info, schema::EventData::TradeSummary, trade_summary.Union());
}

inline flatbuffers::Offset<schema::Event>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::CreateOrderAckEvent& value) {
  auto message_info = convert(fbb, value.message_info);
  auto create_order_ack = convert(fbb, value.create_order_ack);
  return schema::CreateEvent(fbb, message_info, schema::EventData::CreateOrderAck, create_order_ack.Union());
}

inline flatbuffers::Offset<schema::Event>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::ModifyOrderAckEvent& value) {
  auto message_info = convert(fbb, value.message_info);
  auto modify_order_ack = convert(fbb, value.modify_order_ack);
  return schema::CreateEvent(fbb, message_info, schema::EventData::ModifyOrderAck, modify_order_ack.Union());
}

inline flatbuffers::Offset<schema::Event>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::CancelOrderAckEvent& value) {
  auto message_info = convert(fbb, value.message_info);
  auto cancel_order_ack = convert(fbb, value.cancel_order_ack);
  return schema::CreateEvent(fbb, message_info, schema::EventData::CancelOrderAck, cancel_order_ack.Union());
}

inline flatbuffers::Offset<schema::Event>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::OrderUpdateEvent& value) {
  auto message_info = convert(fbb, value.message_info);
  auto order_update = convert(fbb, value.order_update);
  return schema::CreateEvent(fbb, message_info, schema::EventData::OrderUpdate, order_update.Union());
}

inline flatbuffers::Offset<schema::Event>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::TradeUpdateEvent& value) {
  auto message_info = convert(fbb, value.message_info);
  auto trade_update = convert(fbb, value.trade_update);
  return schema::CreateEvent(fbb, message_info, schema::EventData::TradeUpdate, trade_update.Union());
}

// encode (event2)

inline flatbuffers::Offset<schema::Event2>
convert2(flatbuffers::FlatBufferBuilder& fbb, const common::HandshakeAck& value) {
  auto handshake_ack = convert2(fbb, value);
  return schema::CreateEvent2(fbb, schema::EventData::HandshakeAck, handshake_ack.Union());
}

inline flatbuffers::Offset<schema::Event2>
convert2(flatbuffers::FlatBufferBuilder& fbb, const common::HeartbeatAck& value) {
  auto heartbeat_ack = convert2(fbb, value);
  return schema::CreateEvent2(fbb, schema::EventData::HeartbeatAck, heartbeat_ack.Union());
}

inline flatbuffers::Offset<schema::Event2>
convert2(flatbuffers::FlatBufferBuilder& fbb, const common::GatewayStatus& value) {
  auto gateway_status = convert(fbb, value);
  return schema::CreateEvent2(fbb, schema::EventData::GatewayStatus, gateway_status.Union());
}

inline flatbuffers::Offset<schema::Event2>
convert2(flatbuffers::FlatBufferBuilder& fbb, const common::ReferenceData& value) {
  auto reference_data = convert(fbb, value);
  return schema::CreateEvent2(fbb, schema::EventData::ReferenceData, reference_data.Union());
}

inline flatbuffers::Offset<schema::Event2>
convert2(flatbuffers::FlatBufferBuilder& fbb, const common::MarketStatus& value) {
  auto market_status = convert(fbb, value);
  return schema::CreateEvent2(fbb, schema::EventData::MarketStatus, market_status.Union());
}

inline flatbuffers::Offset<schema::Event2>
convert2(flatbuffers::FlatBufferBuilder& fbb, const common::MarketByPrice& value) {
  auto market_by_price = convert(fbb, value);
  return schema::CreateEvent2(fbb, schema::EventData::MarketByPrice, market_by_price.Union());
}

inline flatbuffers::Offset<schema::Event2>
convert2(flatbuffers::FlatBufferBuilder& fbb, const common::TradeSummary& value) {
  auto trade_summary = convert(fbb, value);
  return schema::CreateEvent2(fbb, schema::EventData::TradeSummary, trade_summary.Union());
}

inline flatbuffers::Offset<schema::Event2>
convert2(flatbuffers::FlatBufferBuilder& fbb, const common::CreateOrderAck& value) {
  auto create_order_ack = convert(fbb, value);
  return schema::CreateEvent2(fbb, schema::EventData::CreateOrderAck, create_order_ack.Union());
}

inline flatbuffers::Offset<schema::Event2>
convert2(flatbuffers::FlatBufferBuilder& fbb, const common::ModifyOrderAck& value) {
  auto modify_order_ack = convert(fbb, value);
  return schema::CreateEvent2(fbb, schema::EventData::ModifyOrderAck, modify_order_ack.Union());
}

inline flatbuffers::Offset<schema::Event2>
convert2(flatbuffers::FlatBufferBuilder& fbb, const common::CancelOrderAck& value) {
  auto cancel_order_ack = convert(fbb, value);
  return schema::CreateEvent2(fbb, schema::EventData::CancelOrderAck, cancel_order_ack.Union());
}

inline flatbuffers::Offset<schema::Event2>
convert2(flatbuffers::FlatBufferBuilder& fbb, const common::OrderUpdate& value) {
  auto order_update = convert(fbb, value);
  return schema::CreateEvent2(fbb, schema::EventData::OrderUpdate, order_update.Union());
}

inline flatbuffers::Offset<schema::Event2>
convert2(flatbuffers::FlatBufferBuilder& fbb, const common::TradeUpdate& value) {
  auto trade_update = convert(fbb, value);
  return schema::CreateEvent2(fbb, schema::EventData::TradeUpdate, trade_update.Union());
}

// encode (requests / low level)

inline flatbuffers::Offset<schema::RequestInfo>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::RequestInfo& value) {
  return schema::CreateRequestInfo(
    fbb,
    fbb.CreateString(value.destination),
    fbb.CreateString(value.trace_source),
    value.trace_message_id,
    time_point_to_uint64(value.send_time),
    time_point_to_uint64(value.receive_time));
}

inline flatbuffers::Offset<schema::Handshake>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::Handshake& value) {
  return schema::CreateHandshake(
    fbb,
    fbb.CreateString(value.api_version),
    fbb.CreateString(value.login),
    fbb.CreateString(value.password));
}

inline flatbuffers::Offset<schema::Heartbeat>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::Heartbeat& value) {
  return schema::CreateHeartbeat(
    fbb,
    time_point_to_uint64(value.opaque));
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

inline flatbuffers::Offset<schema::ModifyOrder>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::ModifyOrder& value) {
  return schema::CreateModifyOrder(
    fbb,
    value.order_id,
    value.quantity_change,
    value.limit_price);
}

inline flatbuffers::Offset<schema::CancelOrder>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::CancelOrder& value) {
  return schema::CreateCancelOrder(
    fbb,
    value.order_id);
}

// encode (requests)

inline flatbuffers::Offset<schema::Request>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::HandshakeRequest& value) {
  auto request_info = convert(fbb, value.request_info);
  auto handshake = convert(fbb, value.handshake);
  return schema::CreateRequest(fbb, request_info, schema::RequestData::Handshake, handshake.Union());
}

inline flatbuffers::Offset<schema::Request>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::HeartbeatRequest& value) {
  auto request_info = convert(fbb, value.request_info);
  auto heartbeat = convert(fbb, value.heartbeat);
  return schema::CreateRequest(fbb, request_info, schema::RequestData::Heartbeat, heartbeat.Union());
}

inline flatbuffers::Offset<schema::Request>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::CreateOrderRequest& value) {
  auto request_info = convert(fbb, value.request_info);
  auto create_order = convert(fbb, value.create_order);
  return schema::CreateRequest(fbb, request_info, schema::RequestData::CreateOrder, create_order.Union());
}

inline flatbuffers::Offset<schema::Request>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::ModifyOrderRequest& value) {
  auto request_info = convert(fbb, value.request_info);
  auto modify_order = convert(fbb, value.modify_order);
  return schema::CreateRequest(fbb, request_info, schema::RequestData::ModifyOrder, modify_order.Union());
}

inline flatbuffers::Offset<schema::Request>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::CancelOrderRequest& value) {
  auto request_info = convert(fbb, value.request_info);
  auto cancel_order = convert(fbb, value.cancel_order);
  return schema::CreateRequest(fbb, request_info, schema::RequestData::CancelOrder, cancel_order.Union());
}

// dispatch

class EventDispatcher final {
 public:
  explicit EventDispatcher(Client& client, Strategy& strategy, const MessageInfo *&trace)
      : _client(client), _strategy(strategy), _trace(trace) {}
  // LEGACY
  void dispatch_event(const void *buffer, const size_t length) {
    const auto root = flatbuffers::GetRoot<schema::Event>(buffer);
    const auto message_info = convert(root->message_info());
    assert(message_info.gateway != nullptr);
    _trace = &message_info;
    const auto type = root->event_data_type();
    switch (type) {
      case schema::EventData::HandshakeAck: {
        const auto handshake_ack = convert(root->event_data_as_HandshakeAck());
        const auto event = HandshakeAckEvent{
          .message_info = message_info,
          .handshake_ack = handshake_ack};
        _client.on(event);
        break;
      }
      case schema::EventData::HeartbeatAck: {
        const auto heartbeat_ack = convert(root->event_data_as_HeartbeatAck());
        const auto event = HeartbeatAckEvent{
          .message_info = message_info,
          .heartbeat_ack = heartbeat_ack};
        _client.on(event);
        break;
      }
      case schema::EventData::GatewayStatus: {
        const auto gateway_status = convert(root->event_data_as_GatewayStatus());
        const auto event = GatewayStatusEvent{
          .message_info = message_info,
          .gateway_status = gateway_status};
        _strategy.on(event);
        break;
      }
      case schema::EventData::ReferenceData: {
        const auto reference_data = convert(root->event_data_as_ReferenceData());
        const auto event = ReferenceDataEvent{
          .message_info = message_info,
          .reference_data = reference_data};
        _strategy.on(event);
        break;
      }
      case schema::EventData::MarketStatus: {
        const auto market_status = convert(root->event_data_as_MarketStatus());
        const auto event = MarketStatusEvent{
          .message_info = message_info,
          .market_status = market_status};
        _strategy.on(event);
        break;
      }
      case schema::EventData::MarketByPrice: {
        const auto market_by_price = convert(root->event_data_as_MarketByPrice());
        const auto event = MarketByPriceEvent{
          .message_info = message_info,
          .market_by_price = market_by_price};
        _strategy.on(event);
        break;
      }
      case schema::EventData::TradeSummary: {
        const auto trade_summary = convert(root->event_data_as_TradeSummary());
        const auto event = TradeSummaryEvent{
          .message_info = message_info,
          .trade_summary = trade_summary};
        _strategy.on(event);
        break;
      }
      case schema::EventData::CreateOrderAck: {
        const auto create_order_ack = convert(root->event_data_as_CreateOrderAck());
        const auto event = CreateOrderAckEvent{
          .message_info = message_info,
          .create_order_ack = create_order_ack};
        _strategy.on(event);
        break;
      }
      case schema::EventData::ModifyOrderAck: {
        const auto modify_order_ack = convert(root->event_data_as_ModifyOrderAck());
        const auto event = ModifyOrderAckEvent{
          .message_info = message_info,
          .modify_order_ack = modify_order_ack};
        _strategy.on(event);
        break;
      }
      case schema::EventData::CancelOrderAck: {
        const auto cancel_order_ack = convert(root->event_data_as_CancelOrderAck());
        const auto event = CancelOrderAckEvent{
          .message_info = message_info,
          .cancel_order_ack = cancel_order_ack};
        _strategy.on(event);
        break;
      }
      case schema::EventData::OrderUpdate: {
        const auto order_update = convert(root->event_data_as_OrderUpdate());
        const auto event = OrderUpdateEvent{
          .message_info = message_info,
          .order_update = order_update};
        _strategy.on(event);
        break;
      }
      case schema::EventData::TradeUpdate: {
        const auto trade_update = convert(root->event_data_as_TradeUpdate());
        const auto event = TradeUpdateEvent{
          .message_info = message_info,
          .trade_update = trade_update};
        _strategy.on(event);
        break;
      }
      default: {
        throw std::runtime_error("Unknown message type");
      }
    }
    _trace = nullptr;
  }
  // NEW
  void dispatch_events(const void *buffer, const size_t length) {
    const auto root = flatbuffers::GetRoot<schema::Batch>(buffer);
    const auto message_info = convert(root->message_info());
    assert(message_info.gateway != nullptr);
    _trace = &message_info;
    _strategy.on(BatchBeginEvent{ message_info });
    const auto& events = *(root->events());
    for (int i = 0; i < events.Length(); ++i) {
      const schema::Event2& item = *(events[i]);
      const auto type = item.event_data_type();
      switch (type) {
        case schema::EventData::HandshakeAck: {
          const auto handshake_ack = convert(item.event_data_as_HandshakeAck());
          const auto event = HandshakeAckEvent{
            .message_info = message_info,
            .handshake_ack = handshake_ack};
          _client.on(event);
          break;
        }
        case schema::EventData::HeartbeatAck: {
          const auto heartbeat_ack = convert(item.event_data_as_HeartbeatAck());
          const auto event = HeartbeatAckEvent{
            .message_info = message_info,
            .heartbeat_ack = heartbeat_ack};
          _client.on(event);
          break;
        }
        case schema::EventData::GatewayStatus: {
          const auto gateway_status = convert(item.event_data_as_GatewayStatus());
          const auto event = GatewayStatusEvent{
            .message_info = message_info,
            .gateway_status = gateway_status};
          _strategy.on(event);
          break;
        }
        case schema::EventData::ReferenceData: {
          const auto reference_data = convert(item.event_data_as_ReferenceData());
          const auto event = ReferenceDataEvent{
            .message_info = message_info,
            .reference_data = reference_data};
          _strategy.on(event);
          break;
        }
        case schema::EventData::MarketStatus: {
          const auto market_status = convert(item.event_data_as_MarketStatus());
          const auto event = MarketStatusEvent{
            .message_info = message_info,
            .market_status = market_status};
          _strategy.on(event);
          break;
        }
        case schema::EventData::MarketByPrice: {
          const auto market_by_price = convert(item.event_data_as_MarketByPrice());
          const auto event = MarketByPriceEvent{
            .message_info = message_info,
            .market_by_price = market_by_price};
          _strategy.on(event);
          break;
        }
        case schema::EventData::TradeSummary: {
          const auto trade_summary = convert(item.event_data_as_TradeSummary());
          const auto event = TradeSummaryEvent{
            .message_info = message_info,
            .trade_summary = trade_summary};
          _strategy.on(event);
          break;
        }
        case schema::EventData::CreateOrderAck: {
          const auto create_order_ack = convert(item.event_data_as_CreateOrderAck());
          const auto event = CreateOrderAckEvent{
            .message_info = message_info,
            .create_order_ack = create_order_ack};
          _strategy.on(event);
          break;
        }
        case schema::EventData::ModifyOrderAck: {
          const auto modify_order_ack = convert(item.event_data_as_ModifyOrderAck());
          const auto event = ModifyOrderAckEvent{
            .message_info = message_info,
            .modify_order_ack = modify_order_ack};
          _strategy.on(event);
          break;
        }
        case schema::EventData::CancelOrderAck: {
          const auto cancel_order_ack = convert(item.event_data_as_CancelOrderAck());
          const auto event = CancelOrderAckEvent{
            .message_info = message_info,
            .cancel_order_ack = cancel_order_ack};
          _strategy.on(event);
          break;
        }
        case schema::EventData::OrderUpdate: {
          const auto order_update = convert(item.event_data_as_OrderUpdate());
          const auto event = OrderUpdateEvent{
            .message_info = message_info,
            .order_update = order_update};
          _strategy.on(event);
          break;
        }
        case schema::EventData::TradeUpdate: {
          const auto trade_update = convert(item.event_data_as_TradeUpdate());
          const auto event = TradeUpdateEvent{
            .message_info = message_info,
            .trade_update = trade_update};
          _strategy.on(event);
          break;
        }
        default: {
          throw std::runtime_error("Unknown message type");
        }
      }
    }
    _strategy.on(BatchEndEvent{ .message_info = message_info, });
    _trace = nullptr;  // FIXME(thraneh): also reset when an exception has been raised!
  }

 private:
  EventDispatcher() = delete;
  EventDispatcher(const EventDispatcher&) = delete;
  EventDispatcher& operator=(const EventDispatcher&) = delete;

 private:
  Client& _client;
  Strategy& _strategy;
  const MessageInfo *_trace;
  flatbuffers::FlatBufferBuilder _flat_buffer_builder;
};

class RequestDispatcher final {
 public:
  explicit RequestDispatcher(Server& server, Gateway& gateway) : _server(server), _gateway(gateway) {}
  void dispatch_request(const void *buffer, const size_t length) {
    const auto root = flatbuffers::GetRoot<schema::Request>(buffer);
    const auto request_info = convert(root->request_info());
    const auto type = root->request_data_type();
    switch (type) {
      case schema::RequestData::Handshake: {
        const auto handshake = convert(root->request_data_as_Handshake());
        const auto request = HandshakeRequest{
          .request_info = request_info,
          .handshake = handshake};
        _server.on(request);
        break;
      }
      case schema::RequestData::Heartbeat: {
        const auto heartbeat = convert(root->request_data_as_Heartbeat());
        const auto request = HeartbeatRequest{
          .request_info = request_info,
          .heartbeat = heartbeat};
        _server.on(request);
        break;
      }
      case schema::RequestData::CreateOrder: {
        const auto create_order = convert(root->request_data_as_CreateOrder());
        const auto request = CreateOrderRequest{
          .request_info = request_info,
          .create_order = create_order};
        _gateway.on(request);
        break;
      }
      case schema::RequestData::ModifyOrder: {
        const auto modify_order = convert(root->request_data_as_ModifyOrder());
        const auto request = ModifyOrderRequest{
          .request_info = request_info,
          .modify_order = modify_order};
        _gateway.on(request);
        break;
      }
      case schema::RequestData::CancelOrder: {
        const auto cancel_order = convert(root->request_data_as_CancelOrder());
        const auto request = CancelOrderRequest{
          .request_info = request_info,
          .cancel_order = cancel_order};
        _gateway.on(request);
        break;
      }
      default: {
        throw std::runtime_error("Unknown message type");
      }
    }
  }

 private:
  RequestDispatcher() = delete;
  RequestDispatcher(const RequestDispatcher&) = delete;
  RequestDispatcher& operator=(const RequestDispatcher&) = delete;

 private:
  Server& _server;
  Gateway& _gateway;
  flatbuffers::FlatBufferBuilder _flat_buffer_builder;
};
}  // namespace common
}  // namespace quinclas
