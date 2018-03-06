/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <gtest/gtest.h>
#include <cstdlib>
#include <random>
#include "quinclas/codec.h"

using namespace flatbuffers;  // NOLINT
using namespace quinclas;  // NOLINT

const int MAX_ITERATIONS = 1000;

namespace {
static std::random_device random_device;
static std::mt19937_64 random_engine(random_device());
static std::uniform_int_distribution<uint32_t> random_generator_uint32;
static uint32_t rand_uint32() {
  return random_generator_uint32(random_engine);
}
static std::uniform_int_distribution<uint64_t> random_generator_uint64;
static uint64_t rand_uint64() {
  return random_generator_uint64(random_engine);
}
static bool rand_bool() {
  return (random_generator_uint32(random_engine) % 2) ? true : false;
}
static common::time_point_t rand_time_point() {
  const auto microseconds = random_generator_uint64(random_engine);
  const auto duration = std::chrono::microseconds(microseconds);
  return common::time_point_t(duration);
}
static std::uniform_real_distribution<double> random_generator_double;
static double rand_double() {
  return random_generator_double(random_engine);
}
static std::uniform_int_distribution<int> random_generator_int32;
static int32_t rand_int32() {
  return random_generator_int32(random_engine);
}
}  // namespace

namespace {
static const char * const NAME[] = {"G1", "Gateway2", "SomeVeryLongGatewayName3", ""};
static const size_t NAME_LENGTH = sizeof(NAME) / sizeof(NAME[0]);
}  // namespace

namespace {
static common::ConnectionStatus rand_connection_status() {
  return static_cast<common::ConnectionStatus>(rand_uint32() % static_cast<uint32_t>(common::ConnectionStatus::MAX));
}
static common::LoginStatus rand_login_status() {
  return static_cast<common::LoginStatus>(rand_uint32() % static_cast<uint32_t>(common::LoginStatus::MAX));
}
static common::TradeDirection rand_trade_direction() {
  return static_cast<common::TradeDirection>(rand_uint32() % static_cast<uint32_t>(common::TradeDirection::MAX));
}
static common::TradingStatus rand_trading_status() {
  return static_cast<common::TradingStatus>(rand_uint32() % static_cast<uint32_t>(common::TradingStatus::MAX));
}
static common::OrderStatus rand_order_status() {
  return static_cast<common::OrderStatus>(rand_uint32() % static_cast<uint32_t>(common::OrderStatus::MAX));
}
}  // namespace

namespace {
inline common::MessageInfo CreateRandomMessageInfo() {
  return common::MessageInfo{
    .gateway = NAME[rand_uint32() % NAME_LENGTH],
    .message_id = rand_uint32(),
    .exchange_time = rand_time_point(),
    .receive_time = rand_time_point(),
    .enqueue_time = rand_time_point(),
  };
}
inline common::RequestInfo CreateRandomRequestInfo() {
  return common::RequestInfo{
    .destination = NAME[rand_uint32() % NAME_LENGTH],
    .trace_source = NAME[rand_uint32() % NAME_LENGTH],
    .trace_message_id = rand_uint32(),
    .send_time = rand_time_point(),
    .receive_time = rand_time_point(),
  };
}
inline common::HandshakeAck CreateRandomHandshakeAck() {
  return common::HandshakeAck{
    .api_version = NAME[rand_uint32() % NAME_LENGTH],
    .failure = rand_bool(),
    .reason = NAME[rand_uint32() % NAME_LENGTH],
  };
}
inline common::HeartbeatAck CreateRandomHeartbeatAck() {
  return common::HeartbeatAck{
    .opaque = rand_time_point(),
  };
}
inline common::GatewayStatus CreateRandomGatewayStatus() {
  return common::GatewayStatus{
    .market_data_connection_status = rand_connection_status(),
    .market_data_login_status = rand_login_status(),
    .order_management_connection_status = rand_connection_status(),
    .order_management_login_status = rand_login_status(),
  };
}
inline common::MarketByPrice CreateRandomMarketByPrice() {
  auto res = common::MarketByPrice{
    .exchange = NAME[rand_uint32() % NAME_LENGTH],
    .instrument = NAME[rand_uint32() % NAME_LENGTH],
  };
  for (auto i = 0; i < common::MAX_DEPTH; ++i) {
    res.depth[i].bid_price = rand_double();
    res.depth[i].bid_quantity = rand_double();
    res.depth[i].ask_price = rand_double();
    res.depth[i].ask_quantity = rand_double();
  }
  return res;
}
inline common::TradeSummary CreateRandomTradeSummary() {
  return common::TradeSummary{
    .exchange = NAME[rand_uint32() % NAME_LENGTH],
    .instrument = NAME[rand_uint32() % NAME_LENGTH],
    .price = rand_double(),
    .volume = rand_double(),
    .turnover = rand_double(),
    .direction = rand_trade_direction(),
  };
}
inline common::ReferenceData CreateRandomReferenceData() {
  return common::ReferenceData{
    .exchange = NAME[rand_uint32() % NAME_LENGTH],
    .instrument = NAME[rand_uint32() % NAME_LENGTH],
    .tick_size = rand_double(),
    .limit_up = rand_double(),
    .limit_down = rand_double(),
  };
}
inline common::MarketStatus CreateRandomMarketStatus() {
  return common::MarketStatus{
    .exchange = NAME[rand_uint32() % NAME_LENGTH],
    .instrument = NAME[rand_uint32() % NAME_LENGTH],
    .trading_status = rand_trading_status(),
  };
}
inline common::CreateOrderAck CreateRandomCreateOrderAck() {
  return common::CreateOrderAck{
    .opaque = rand_uint32(),
    .order_id = rand_uint32(),
    .failure = rand_bool(),
    .reason = NAME[rand_uint32() % NAME_LENGTH],
    .order_template = NAME[rand_uint32() % NAME_LENGTH],
    .external_order_id = NAME[rand_uint32() % NAME_LENGTH],
    .exchange = NAME[rand_uint32() % NAME_LENGTH],
    .instrument = NAME[rand_uint32() % NAME_LENGTH],
  };
}
inline common::ModifyOrderAck CreateRandomModifyOrderAck() {
  return common::ModifyOrderAck{
    .opaque = rand_uint32(),
    .order_id = rand_uint32(),
    .failure = rand_bool(),
    .reason = NAME[rand_uint32() % NAME_LENGTH],
    .order_template = NAME[rand_uint32() % NAME_LENGTH],
    .external_order_id = NAME[rand_uint32() % NAME_LENGTH],
    .exchange = NAME[rand_uint32() % NAME_LENGTH],
    .instrument = NAME[rand_uint32() % NAME_LENGTH],
    .quantity_change = rand_double(),
    .limit_price = rand_double(),
  };
}
inline common::CancelOrderAck CreateRandomCancelOrderAck() {
  return common::CancelOrderAck{
    .opaque = rand_uint32(),
    .order_id = rand_uint32(),
    .failure = rand_bool(),
    .reason = NAME[rand_uint32() % NAME_LENGTH],
    .order_template = NAME[rand_uint32() % NAME_LENGTH],
    .external_order_id = NAME[rand_uint32() % NAME_LENGTH],
    .exchange = NAME[rand_uint32() % NAME_LENGTH],
    .instrument = NAME[rand_uint32() % NAME_LENGTH],
  };
}
inline common::OrderUpdate CreateRandomOrderUpdate() {
  return common::OrderUpdate{
    .opaque = rand_uint32(),
    .order_id = rand_uint32(),
    .order_template = NAME[rand_uint32() % NAME_LENGTH],
    .external_order_id = NAME[rand_uint32() % NAME_LENGTH],
    .exchange = NAME[rand_uint32() % NAME_LENGTH],
    .instrument = NAME[rand_uint32() % NAME_LENGTH],
    .status = rand_order_status(),
    .trade_direction = rand_trade_direction(),
    .remaining_quantity = rand_double(),
    .traded_quantity = rand_double(),
    .insert_time = rand_time_point(),
    .cancel_time = rand_time_point(),
  };
}
inline common::TradeUpdate CreateRandomTradeUpdate() {
  return common::TradeUpdate{
    .order_id = rand_uint32(),
    .external_order_id = NAME[rand_uint32() % NAME_LENGTH],
    .external_trade_id = NAME[rand_uint32() % NAME_LENGTH],
    .exchange = NAME[rand_uint32() % NAME_LENGTH],
    .instrument = NAME[rand_uint32() % NAME_LENGTH],
    .trade_direction = rand_trade_direction(),
    .quantity = rand_double(),
    .price = rand_double(),
    .trade_time = rand_time_point(),
  };
}
inline common::Handshake CreateRandomHandshake() {
  return common::Handshake{
    .api_version = NAME[rand_uint32() % NAME_LENGTH],
    .login = NAME[rand_uint32() % NAME_LENGTH],
    .password = NAME[rand_uint32() % NAME_LENGTH],
  };
}
inline common::Heartbeat CreateRandomHeartbeat() {
  return common::Heartbeat{
    .opaque = rand_time_point(),
  };
}
inline common::CreateOrder CreateRandomCreateOrder() {
  return common::CreateOrder{
    .opaque = rand_uint32(),
    .order_template = NAME[rand_uint32() % NAME_LENGTH],
    .exchange = NAME[rand_uint32() % NAME_LENGTH],
    .instrument = NAME[rand_uint32() % NAME_LENGTH],
    .direction = rand_trade_direction(),
    .quantity = rand_double(),
    .limit_price = rand_double(),
    .stop_price = rand_double(),
  };
}
inline common::ModifyOrder CreateRandomModifyOrder() {
  return common::ModifyOrder{
    .order_id = rand_uint32(),
    .quantity_change = rand_double(),
    .limit_price = rand_double(),
  };
}
inline common::CancelOrder CreateRandomCancelOrder() {
  return common::CancelOrder{
    .order_id = rand_uint32(),
  };
}
}  // namespace

namespace {
void compare(const common::MessageInfo& lhs, const common::MessageInfo& rhs) {
  EXPECT_STREQ(lhs.gateway, rhs.gateway);
  EXPECT_EQ(lhs.message_id, rhs.message_id);
  EXPECT_EQ(lhs.exchange_time, rhs.exchange_time);
  EXPECT_EQ(lhs.receive_time, rhs.receive_time);
  EXPECT_EQ(lhs.enqueue_time, rhs.enqueue_time);
}
void compare(const common::RequestInfo& lhs, const common::RequestInfo& rhs) {
  EXPECT_STREQ(lhs.destination, rhs.destination);
  EXPECT_STREQ(lhs.trace_source, rhs.trace_source);
  EXPECT_EQ(lhs.trace_message_id, rhs.trace_message_id);
  EXPECT_EQ(lhs.send_time, rhs.send_time);
  EXPECT_EQ(lhs.receive_time, rhs.receive_time);
}
void compare(const common::HandshakeAck& lhs, const common::HandshakeAck& rhs) {
  EXPECT_STREQ(lhs.api_version, rhs.api_version);
  EXPECT_EQ(lhs.failure, rhs.failure);
  EXPECT_STREQ(lhs.reason, rhs.reason);
}
void compare(const common::HeartbeatAck& lhs, const common::HeartbeatAck& rhs) {
  EXPECT_EQ(lhs.opaque, rhs.opaque);
}
void compare(const common::GatewayStatus& lhs, const common::GatewayStatus& rhs) {
  EXPECT_EQ(lhs.market_data_connection_status, rhs.market_data_connection_status);
  EXPECT_EQ(lhs.market_data_login_status, rhs.market_data_login_status);
  EXPECT_EQ(lhs.order_management_connection_status, rhs.order_management_connection_status);
  EXPECT_EQ(lhs.order_management_login_status, rhs.order_management_login_status);
}
void compare(const common::MarketByPrice& lhs, const common::MarketByPrice& rhs) {
  EXPECT_STREQ(lhs.exchange, rhs.exchange);
  EXPECT_STREQ(lhs.instrument, rhs.instrument);
  for (auto i = 0; i < common::MAX_DEPTH; ++i) {
    EXPECT_EQ(lhs.depth[i].bid_price, rhs.depth[i].bid_price);
    EXPECT_EQ(lhs.depth[i].bid_quantity, rhs.depth[i].bid_quantity);
    EXPECT_EQ(lhs.depth[i].ask_price, rhs.depth[i].ask_price);
    EXPECT_EQ(lhs.depth[i].ask_quantity, rhs.depth[i].ask_quantity);
  }
}
void compare(const common::TradeSummary& lhs, const common::TradeSummary& rhs) {
  EXPECT_STREQ(lhs.exchange, rhs.exchange);
  EXPECT_STREQ(lhs.instrument, rhs.instrument);
  EXPECT_EQ(lhs.price, rhs.price);
  EXPECT_EQ(lhs.volume, rhs.volume);
  EXPECT_EQ(lhs.turnover, rhs.turnover);
  EXPECT_EQ(lhs.direction, rhs.direction);
}
void compare(const common::ReferenceData& lhs, const common::ReferenceData& rhs) {
  EXPECT_STREQ(lhs.exchange, rhs.exchange);
  EXPECT_STREQ(lhs.instrument, rhs.instrument);
  EXPECT_EQ(lhs.tick_size, rhs.tick_size);
  EXPECT_EQ(lhs.limit_up, rhs.limit_up);
  EXPECT_EQ(lhs.limit_down, rhs.limit_down);
}
void compare(const common::MarketStatus& lhs, const common::MarketStatus& rhs) {
  EXPECT_STREQ(lhs.exchange, rhs.exchange);
  EXPECT_STREQ(lhs.instrument, rhs.instrument);
  EXPECT_EQ(lhs.trading_status, rhs.trading_status);
}
void compare(const common::CreateOrderAck& lhs, const common::CreateOrderAck& rhs) {
  EXPECT_EQ(lhs.opaque, rhs.opaque);
  EXPECT_EQ(lhs.order_id, rhs.order_id);
  EXPECT_EQ(lhs.failure, rhs.failure);
  EXPECT_STREQ(lhs.reason, rhs.reason);
  EXPECT_STREQ(lhs.order_template, rhs.order_template);
  EXPECT_STREQ(lhs.external_order_id, rhs.external_order_id);
  EXPECT_STREQ(lhs.exchange, rhs.exchange);
  EXPECT_STREQ(lhs.instrument, rhs.instrument);
}
void compare(const common::ModifyOrderAck& lhs, const common::ModifyOrderAck& rhs) {
  EXPECT_EQ(lhs.opaque, rhs.opaque);
  EXPECT_EQ(lhs.order_id, rhs.order_id);
  EXPECT_EQ(lhs.failure, rhs.failure);
  EXPECT_STREQ(lhs.reason, rhs.reason);
  EXPECT_STREQ(lhs.order_template, rhs.order_template);
  EXPECT_STREQ(lhs.external_order_id, rhs.external_order_id);
  EXPECT_STREQ(lhs.exchange, rhs.exchange);
  EXPECT_STREQ(lhs.instrument, rhs.instrument);
  EXPECT_EQ(lhs.quantity_change, rhs.quantity_change);
  EXPECT_EQ(lhs.limit_price, rhs.limit_price);
}
void compare(const common::CancelOrderAck& lhs, const common::CancelOrderAck& rhs) {
  EXPECT_EQ(lhs.opaque, rhs.opaque);
  EXPECT_EQ(lhs.order_id, rhs.order_id);
  EXPECT_EQ(lhs.failure, rhs.failure);
  EXPECT_STREQ(lhs.reason, rhs.reason);
  EXPECT_STREQ(lhs.order_template, rhs.order_template);
  EXPECT_STREQ(lhs.external_order_id, rhs.external_order_id);
  EXPECT_STREQ(lhs.exchange, rhs.exchange);
  EXPECT_STREQ(lhs.instrument, rhs.instrument);
}
void compare(const common::OrderUpdate& lhs, const common::OrderUpdate& rhs) {
  EXPECT_EQ(lhs.opaque, rhs.opaque);
  EXPECT_EQ(lhs.order_id, rhs.order_id);
  EXPECT_STREQ(lhs.order_template, rhs.order_template);
  EXPECT_STREQ(lhs.external_order_id, rhs.external_order_id);
  EXPECT_STREQ(lhs.exchange, rhs.exchange);
  EXPECT_STREQ(lhs.instrument, rhs.instrument);
  EXPECT_EQ(lhs.status, rhs.status);
  EXPECT_EQ(lhs.trade_direction, rhs.trade_direction);
  EXPECT_EQ(lhs.remaining_quantity, rhs.remaining_quantity);
  EXPECT_EQ(lhs.traded_quantity, rhs.traded_quantity);
  EXPECT_EQ(lhs.insert_time, rhs.insert_time);
  EXPECT_EQ(lhs.cancel_time, rhs.cancel_time);
}
void compare(const common::TradeUpdate& lhs, const common::TradeUpdate& rhs) {
  EXPECT_EQ(lhs.order_id, rhs.order_id);
  EXPECT_STREQ(lhs.external_order_id, rhs.external_order_id);
  EXPECT_STREQ(lhs.external_trade_id, rhs.external_trade_id);
  EXPECT_STREQ(lhs.exchange, rhs.exchange);
  EXPECT_STREQ(lhs.instrument, rhs.instrument);
  EXPECT_EQ(lhs.trade_direction, rhs.trade_direction);
  EXPECT_EQ(lhs.quantity, rhs.quantity);
  EXPECT_EQ(lhs.price, rhs.price);
  EXPECT_EQ(lhs.trade_time, rhs.trade_time);
}
void compare(const common::Handshake& lhs, const common::Handshake& rhs) {
  EXPECT_STREQ(lhs.api_version, rhs.api_version);
  EXPECT_STREQ(lhs.login, rhs.login);
  EXPECT_STREQ(lhs.password, rhs.password);
}
void compare(const common::Heartbeat& lhs, const common::Heartbeat& rhs) {
  EXPECT_EQ(lhs.opaque, rhs.opaque);
}
void compare(const common::CreateOrder& lhs, const common::CreateOrder& rhs) {
  EXPECT_EQ(lhs.opaque, rhs.opaque);
  EXPECT_STREQ(lhs.order_template, rhs.order_template);
  EXPECT_STREQ(lhs.exchange, rhs.exchange);
  EXPECT_STREQ(lhs.instrument, rhs.instrument);
  EXPECT_EQ(lhs.direction, rhs.direction);
  EXPECT_EQ(lhs.quantity, rhs.quantity);
  EXPECT_EQ(lhs.limit_price, rhs.limit_price);
  EXPECT_EQ(lhs.stop_price, rhs.stop_price);
}
void compare(const common::ModifyOrder& lhs, const common::ModifyOrder& rhs) {
  EXPECT_EQ(lhs.order_id, rhs.order_id);
  EXPECT_EQ(lhs.quantity_change, rhs.quantity_change);
  EXPECT_EQ(lhs.limit_price, rhs.limit_price);
}
void compare(const common::CancelOrder& lhs, const common::CancelOrder& rhs) {
  EXPECT_EQ(lhs.order_id, rhs.order_id);
}
}  // namespace

TEST(flatbuffers, handshake_ack_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    const auto source_message_info = CreateRandomMessageInfo();
    const auto source_handshake_ack = CreateRandomHandshakeAck();
    const auto source = common::HandshakeAckEvent{
      .message_info = source_message_info,
      .handshake_ack = source_handshake_ack};
    // serialize using flatbuffers
    fbb.Finish(common::convert(fbb, source));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    const std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    const auto root = GetRoot<schema::Event>(&buffer[0]);
    const auto target_message_info = common::convert(root->message_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::HandshakeAck);
    const auto target_handshake_ack = common::convert(root->event_data_as_HandshakeAck());
    // event (target)
    const auto target = common::HandshakeAckEvent{
      .message_info = target_message_info,
      .handshake_ack = target_handshake_ack};
    // validate
    compare(target.message_info, source.message_info);
    compare(target.handshake_ack, source.handshake_ack);
  }
}

TEST(flatbuffers, heartbeat_ack_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    const auto source_message_info = CreateRandomMessageInfo();
    const auto source_heartbeat_ack = CreateRandomHeartbeatAck();
    const auto source = common::HeartbeatAckEvent{
      .message_info = source_message_info,
      .heartbeat_ack = source_heartbeat_ack};
    // serialize using flatbuffers
    fbb.Finish(common::convert(fbb, source));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    const std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    const auto root = GetRoot<schema::Event>(&buffer[0]);
    const auto target_message_info = common::convert(root->message_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::HeartbeatAck);
    const auto target_heartbeat_ack = common::convert(root->event_data_as_HeartbeatAck());
    // event (target)
    const auto target = common::HeartbeatAckEvent{
      .message_info = target_message_info,
      .heartbeat_ack = target_heartbeat_ack};
    // validate
    compare(target.message_info, source.message_info);
    compare(target.heartbeat_ack, source.heartbeat_ack);
  }
}

TEST(flatbuffers, gateway_status_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    const auto source_message_info = CreateRandomMessageInfo();
    const auto source_gateway_status = CreateRandomGatewayStatus();
    const auto source = common::GatewayStatusEvent{
      .message_info = source_message_info,
      .gateway_status = source_gateway_status};
    // serialize using flatbuffers
    fbb.Finish(common::convert(fbb, source));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    const std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    const auto root = GetRoot<schema::Event>(&buffer[0]);
    const auto target_message_info = common::convert(root->message_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::GatewayStatus);
    const auto target_gateway_status = common::convert(root->event_data_as_GatewayStatus());
    // event (target)
    const auto target = common::GatewayStatusEvent{
      .message_info = target_message_info,
      .gateway_status = target_gateway_status};
    // validate
    compare(target.message_info, source.message_info);
    compare(target.gateway_status, source.gateway_status);
  }
}

TEST(flatbuffers, market_by_price_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    const auto source_message_info = CreateRandomMessageInfo();
    const auto source_market_by_price = CreateRandomMarketByPrice();
    const auto source = common::MarketByPriceEvent{
      .message_info = source_message_info,
      .market_by_price = source_market_by_price};
    // serialize using flatbuffers
    fbb.Finish(common::convert(fbb, source));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    const std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    const auto root = GetRoot<schema::Event>(&buffer[0]);
    const auto target_message_info = common::convert(root->message_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::MarketByPrice);
    const auto target_market_by_price = common::convert(root->event_data_as_MarketByPrice());
    // event (target)
    const auto target = common::MarketByPriceEvent{
      .message_info = target_message_info,
      .market_by_price = target_market_by_price};
    // validate
    compare(target.message_info, source.message_info);
    compare(target.market_by_price, source.market_by_price);
  }
}

TEST(flatbuffers, trade_summary_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    const auto source_message_info = CreateRandomMessageInfo();
    const auto source_trade_summary = CreateRandomTradeSummary();
    const auto source = common::TradeSummaryEvent{
      .message_info = source_message_info,
      .trade_summary = source_trade_summary};
    // serialize using flatbuffers
    fbb.Finish(common::convert(fbb, source));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    const std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    const auto root = GetRoot<schema::Event>(&buffer[0]);
    const auto target_message_info = common::convert(root->message_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::TradeSummary);
    const auto target_trade_summary = common::convert(root->event_data_as_TradeSummary());
    // event (target)
    const auto target = common::TradeSummaryEvent{
      .message_info = target_message_info,
      .trade_summary = target_trade_summary};
    // validate
    compare(target.message_info, source.message_info);
    compare(target.trade_summary, source.trade_summary);
  }
}

TEST(flatbuffers, reference_data_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    const auto source_message_info = CreateRandomMessageInfo();
    const auto source_reference_data = CreateRandomReferenceData();
    const auto source = common::ReferenceDataEvent{
      .message_info = source_message_info,
      .reference_data = source_reference_data};
    // serialize using flatbuffers
    fbb.Finish(common::convert(fbb, source));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    const std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    const auto root = GetRoot<schema::Event>(&buffer[0]);
    const auto target_message_info = common::convert(root->message_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::ReferenceData);
    const auto target_reference_data = common::convert(root->event_data_as_ReferenceData());
    // event (target)
    const auto target = common::ReferenceDataEvent{
      .message_info = target_message_info,
      .reference_data = target_reference_data};
    // validate
    compare(target.message_info, source.message_info);
    compare(target.reference_data, source.reference_data);
  }
}

TEST(flatbuffers, market_status_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    const auto source_message_info = CreateRandomMessageInfo();
    const auto source_market_status = CreateRandomMarketStatus();
    const auto source = common::MarketStatusEvent{
      .message_info = source_message_info,
      .market_status = source_market_status};
    // serialize using flatbuffers
    fbb.Finish(common::convert(fbb, source));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    const std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    const auto root = GetRoot<schema::Event>(&buffer[0]);
    const auto target_message_info = common::convert(root->message_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::MarketStatus);
    const auto target_market_status = common::convert(root->event_data_as_MarketStatus());
    // event (target)
    const auto target = common::MarketStatusEvent{
      .message_info = target_message_info,
      .market_status = target_market_status};
    // validate
    compare(target.message_info, source.message_info);
    compare(target.market_status, source.market_status);
  }
}

TEST(flatbuffers, create_order_ack_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    const auto source_message_info = CreateRandomMessageInfo();
    const auto source_create_order_ack = CreateRandomCreateOrderAck();
    const auto source = common::CreateOrderAckEvent{
      .message_info = source_message_info,
      .create_order_ack = source_create_order_ack};
    // serialize using flatbuffers
    fbb.Finish(common::convert(fbb, source));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    const std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    const auto root = GetRoot<schema::Event>(&buffer[0]);
    const auto target_message_info = common::convert(root->message_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::CreateOrderAck);
    const auto target_create_order_ack = common::convert(root->event_data_as_CreateOrderAck());
    // event (target)
    const auto target = common::CreateOrderAckEvent{
      .message_info = target_message_info,
      .create_order_ack = target_create_order_ack};
    // validate
    compare(target.message_info, source.message_info);
    compare(target.create_order_ack, source.create_order_ack);
  }
}

TEST(flatbuffers, modify_order_ack_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    const auto source_message_info = CreateRandomMessageInfo();
    const auto source_modify_order_ack = CreateRandomModifyOrderAck();
    const auto source = common::ModifyOrderAckEvent{
      .message_info = source_message_info,
      .modify_order_ack = source_modify_order_ack};
    // serialize using flatbuffers
    fbb.Finish(common::convert(fbb, source));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    const std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    const auto root = GetRoot<schema::Event>(&buffer[0]);
    const auto target_message_info = common::convert(root->message_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::ModifyOrderAck);
    const auto target_modify_order_ack = common::convert(root->event_data_as_ModifyOrderAck());
    // event (target)
    const auto target = common::ModifyOrderAckEvent{
      .message_info = target_message_info,
      .modify_order_ack = target_modify_order_ack};
    // validate
    compare(target.message_info, source.message_info);
    compare(target.modify_order_ack, source.modify_order_ack);
  }
}

TEST(flatbuffers, cancel_order_ack_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    const auto source_message_info = CreateRandomMessageInfo();
    const auto source_cancel_order_ack = CreateRandomCancelOrderAck();
    const auto source = common::CancelOrderAckEvent{
      .message_info = source_message_info,
      .cancel_order_ack = source_cancel_order_ack};
    // serialize using flatbuffers
    fbb.Finish(common::convert(fbb, source));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    const std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    const auto root = GetRoot<schema::Event>(&buffer[0]);
    const auto target_message_info = common::convert(root->message_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::CancelOrderAck);
    const auto target_cancel_order_ack = common::convert(root->event_data_as_CancelOrderAck());
    // event (target)
    const auto target = common::CancelOrderAckEvent{
      .message_info = target_message_info,
      .cancel_order_ack = target_cancel_order_ack};
    // validate
    compare(target.message_info, source.message_info);
    compare(target.cancel_order_ack, source.cancel_order_ack);
  }
}

TEST(flatbuffers, order_update_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    const auto source_message_info = CreateRandomMessageInfo();
    const auto source_order_update = CreateRandomOrderUpdate();
    const auto source = common::OrderUpdateEvent{
      .message_info = source_message_info,
      .order_update = source_order_update};
    // serialize using flatbuffers
    fbb.Finish(common::convert(fbb, source));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    const std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    const auto root = GetRoot<schema::Event>(&buffer[0]);
    const auto target_message_info = common::convert(root->message_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::OrderUpdate);
    const auto target_order_update = common::convert(root->event_data_as_OrderUpdate());
    // event (target)
    const auto target = common::OrderUpdateEvent{
      .message_info = target_message_info,
      .order_update = target_order_update};
    // validate
    compare(target.message_info, source.message_info);
    compare(target.order_update, source.order_update);
  }
}

TEST(flatbuffers, trade_update_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    const auto source_message_info = CreateRandomMessageInfo();
    const auto source_trade_update = CreateRandomTradeUpdate();
    const auto source = common::TradeUpdateEvent{
      .message_info = source_message_info,
      .trade_update = source_trade_update};
    // serialize using flatbuffers
    fbb.Finish(common::convert(fbb, source));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    const std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    const auto root = GetRoot<schema::Event>(&buffer[0]);
    const auto target_message_info = common::convert(root->message_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::TradeUpdate);
    const auto target_trade_update = common::convert(root->event_data_as_TradeUpdate());
    // event (target)
    const auto target = common::TradeUpdateEvent{
      .message_info = target_message_info,
      .trade_update = target_trade_update};
    // validate
    compare(target.message_info, source.message_info);
    compare(target.trade_update, source.trade_update);
  }
}

TEST(flatbuffers, handshake_request) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // request (source)
    const auto source_request_info = CreateRandomRequestInfo();
    const auto source_handshake = CreateRandomHandshake();
    const auto source = common::HandshakeRequest{
      .request_info = source_request_info,
      .handshake = source_handshake};
    // serialize using flatbuffers
    fbb.Finish(common::convert(fbb, source));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    const std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    const auto root = GetRoot<schema::Request>(&buffer[0]);
    const auto target_request_info = common::convert(root->request_info());
    EXPECT_EQ(root->request_data_type(), schema::RequestData::Handshake);
    const auto target_handshake = common::convert(root->request_data_as_Handshake());
    // request (target)
    const auto target = common::HandshakeRequest{
      .request_info = target_request_info,
      .handshake = target_handshake};
    // validate
    compare(target.request_info, source.request_info);
    compare(target.handshake, source.handshake);
  }
}

TEST(flatbuffers, heartbeat_request) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // request (source)
    const auto source_request_info = CreateRandomRequestInfo();
    const auto source_heartbeat = CreateRandomHeartbeat();
    const auto source = common::HeartbeatRequest{
      .request_info = source_request_info,
      .heartbeat = source_heartbeat};
    // serialize using flatbuffers
    fbb.Finish(common::convert(fbb, source));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    const std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    const auto root = GetRoot<schema::Request>(&buffer[0]);
    const auto target_request_info = common::convert(root->request_info());
    EXPECT_EQ(root->request_data_type(), schema::RequestData::Heartbeat);
    const auto target_heartbeat = common::convert(root->request_data_as_Heartbeat());
    // request (target)
    const auto target = common::HeartbeatRequest{
      .request_info = target_request_info,
      .heartbeat = target_heartbeat};
    // validate
    compare(target.request_info, source.request_info);
    compare(target.heartbeat, source.heartbeat);
  }
}

TEST(flatbuffers, create_order_request) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // request (source)
    const auto source_request_info = CreateRandomRequestInfo();
    const auto source_create_order = CreateRandomCreateOrder();
    const auto source = common::CreateOrderRequest{
      .request_info = source_request_info,
      .create_order = source_create_order};
    // serialize using flatbuffers
    fbb.Finish(common::convert(fbb, source));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    const std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    const auto root = GetRoot<schema::Request>(&buffer[0]);
    const auto target_request_info = common::convert(root->request_info());
    EXPECT_EQ(root->request_data_type(), schema::RequestData::CreateOrder);
    const auto target_create_order = common::convert(root->request_data_as_CreateOrder());
    // request (target)
    const auto target = common::CreateOrderRequest{
      .request_info = target_request_info,
      .create_order = target_create_order};
    // validate
    compare(target.request_info, source.request_info);
    compare(target.create_order, source.create_order);
  }
}

TEST(flatbuffers, modify_order_request) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // request (source)
    const auto source_request_info = CreateRandomRequestInfo();
    const auto source_modify_order = CreateRandomModifyOrder();
    const auto source = common::ModifyOrderRequest{
      .request_info = source_request_info,
      .modify_order = source_modify_order};
    // serialize using flatbuffers
    fbb.Finish(common::convert(fbb, source));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    const std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    const auto root = GetRoot<schema::Request>(&buffer[0]);
    const auto target_request_info = common::convert(root->request_info());
    EXPECT_EQ(root->request_data_type(), schema::RequestData::ModifyOrder);
    const auto target_modify_order = common::convert(root->request_data_as_ModifyOrder());
    // request (target)
    const auto target = common::ModifyOrderRequest{
      .request_info = target_request_info,
      .modify_order = target_modify_order};
    // validate
    compare(target.request_info, source.request_info);
    compare(target.modify_order, source.modify_order);
  }
}

TEST(flatbuffers, cancel_order_request) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // request (source)
    const auto source_request_info = CreateRandomRequestInfo();
    const auto source_cancel_order = CreateRandomCancelOrder();
    const auto source = common::CancelOrderRequest{
      .request_info = source_request_info,
      .cancel_order = source_cancel_order};
    // serialize using flatbuffers
    fbb.Finish(common::convert(fbb, source));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    const std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    const auto root = GetRoot<schema::Request>(&buffer[0]);
    const auto target_request_info = common::convert(root->request_info());
    EXPECT_EQ(root->request_data_type(), schema::RequestData::CancelOrder);
    const auto target_cancel_order = common::convert(root->request_data_as_CancelOrder());
    // request (target)
    const auto target = common::CancelOrderRequest{
      .request_info = target_request_info,
      .cancel_order = target_cancel_order};
    // validate
    compare(target.request_info, source.request_info);
    compare(target.cancel_order, source.cancel_order);
  }
}
