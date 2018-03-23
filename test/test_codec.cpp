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
static std::uniform_int_distribution<int64_t> random_generator_int64;
static int64_t rand_int64() {
  return random_generator_int64(random_engine);
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
static common::GatewayState rand_gateway_state() {
  return static_cast<common::GatewayState>(rand_uint32() % static_cast<uint32_t>(common::GatewayState::MAX));
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
static std::vector<std::string> rand_vector_string(size_t max_length = 100) {
  auto length = rand_uint32() % max_length;
  std::vector<std::string> result(length);
  if (length != 0) {
    result.resize(length);
    for (auto i = 0; i < length; ++i)
      result[i] = NAME[rand_uint32() % NAME_LENGTH];
  }
  return result;
}
}  // namespace

namespace {
inline common::SourceInfo CreateRandomSourceInfo() {
  return common::SourceInfo {
    .seqno = rand_uint64(),
    .create_time = rand_time_point()
  };
}
inline common::Handshake CreateRandomHandshake() {
  return common::Handshake {
    .api_version = NAME[rand_uint32() % NAME_LENGTH],
    .application = NAME[rand_uint32() % NAME_LENGTH],
    .hostname = NAME[rand_uint32() % NAME_LENGTH],
    .pid = rand_uint32(),
    .uuid = NAME[rand_uint32() % NAME_LENGTH],
    .login = NAME[rand_uint32() % NAME_LENGTH],
    .password = NAME[rand_uint32() % NAME_LENGTH],
    .subscriptions = rand_vector_string(),
  };
}
inline common::HandshakeAck CreateRandomHandshakeAck() {
  return common::HandshakeAck {
    .api_version = NAME[rand_uint32() % NAME_LENGTH],
    .application = NAME[rand_uint32() % NAME_LENGTH],
    .hostname = NAME[rand_uint32() % NAME_LENGTH],
    .pid = rand_uint32(),
    .failure = rand_bool(),
    .failure_reason = NAME[rand_uint32() % NAME_LENGTH],
    .server_uuid = NAME[rand_uint32() % NAME_LENGTH],
    .server_name = NAME[rand_uint32() % NAME_LENGTH],
    .shmem_s2c_name = NAME[rand_uint32() % NAME_LENGTH],
    .shmem_s2c_size = rand_uint32(),
    .shmem_c2s_name = NAME[rand_uint32() % NAME_LENGTH],
    .shmem_c2s_size = rand_uint32(),
  };
}
inline common::Heartbeat CreateRandomHeartbeat() {
  return common::Heartbeat {
    .opaque = rand_int64(),
  };
}
inline common::HeartbeatAck CreateRandomHeartbeatAck() {
  return common::HeartbeatAck {
    .opaque = rand_int64(),
  };
}
inline common::Ready CreateRandomReady() {
  return common::Ready {};
}
inline common::GatewayStatus CreateRandomGatewayStatus() {
  return common::GatewayStatus {
    .name = NAME[rand_uint32() % NAME_LENGTH],
    .status = rand_gateway_state(),
  };
}
inline common::MarketByPrice CreateRandomMarketByPrice() {
  common::MarketByPrice res {
    .exchange = NAME[rand_uint32() % NAME_LENGTH],
    .instrument = NAME[rand_uint32() % NAME_LENGTH],
  };
  for (auto i = 0; i < common::MAX_DEPTH; ++i) {
    res.depth[i].bid_price = rand_double();
    res.depth[i].bid_quantity = rand_double();
    res.depth[i].ask_price = rand_double();
    res.depth[i].ask_quantity = rand_double();
  }
  res.exchange_time = rand_time_point();
  res.channel = rand_uint32();
  return res;
}
inline common::TradeSummary CreateRandomTradeSummary() {
  return common::TradeSummary {
    .exchange = NAME[rand_uint32() % NAME_LENGTH],
    .instrument = NAME[rand_uint32() % NAME_LENGTH],
    .price = rand_double(),
    .volume = rand_double(),
    .turnover = rand_double(),
    .direction = rand_trade_direction(),
    .exchange_time = rand_time_point(),
    .channel = rand_uint32(),
  };
}
inline common::ReferenceData CreateRandomReferenceData() {
  return common::ReferenceData {
    .exchange = NAME[rand_uint32() % NAME_LENGTH],
    .instrument = NAME[rand_uint32() % NAME_LENGTH],
    .tick_size = rand_double(),
    .limit_up = rand_double(),
    .limit_down = rand_double(),
  };
}
inline common::MarketStatus CreateRandomMarketStatus() {
  return common::MarketStatus {
    .exchange = NAME[rand_uint32() % NAME_LENGTH],
    .instrument = NAME[rand_uint32() % NAME_LENGTH],
    .trading_status = rand_trading_status(),
  };
}
inline common::CreateOrder CreateRandomCreateOrder() {
  return common::CreateOrder {
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
inline common::CreateOrderAck CreateRandomCreateOrderAck() {
  return common::CreateOrderAck {
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
inline common::ModifyOrder CreateRandomModifyOrder() {
  return common::ModifyOrder {
    .order_id = rand_uint32(),
    .quantity_change = rand_double(),
    .limit_price = rand_double(),
  };
}
inline common::ModifyOrderAck CreateRandomModifyOrderAck() {
  return common::ModifyOrderAck {
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
inline common::CancelOrder CreateRandomCancelOrder() {
  return common::CancelOrder {
    .order_id = rand_uint32(),
  };
}
inline common::CancelOrderAck CreateRandomCancelOrderAck() {
  return common::CancelOrderAck {
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
  return common::OrderUpdate {
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
  return common::TradeUpdate {
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
inline common::PositionUpdate CreateRandomPositionUpdate() {
  return common::PositionUpdate {
    .exchange = NAME[rand_uint32() % NAME_LENGTH],
    .instrument = NAME[rand_uint32() % NAME_LENGTH],
    .trade_direction = rand_trade_direction(),
    .position = rand_double(),
    .position_yesterday = rand_double(),
    .frozen_position = rand_double(),
    .frozen_closing = rand_double(),
    .frozen_closing_yesterday = rand_double(),
  };
}
}  // namespace

namespace {
// comparing time-points must go through normalisation
// for example, if we use system_clock::now(), we may have a higher
// resolution than the one we use on-the-wire, in other words, we
// may truncate a time-point when we encode it
void compare(common::time_point_t lhs, common::time_point_t rhs) {
  auto lhs_adj = common::uint64_to_time_point(
      common::time_point_to_uint64(lhs));
  auto rhs_adj = common::uint64_to_time_point(
      common::time_point_to_uint64(rhs));
  EXPECT_EQ(lhs_adj, rhs_adj);
}
void compare(const common::SourceInfo& lhs, const common::SourceInfo& rhs) {
  EXPECT_EQ(lhs.seqno, rhs.seqno);
  compare(lhs.create_time, rhs.create_time);
}
void compare(const common::Handshake& lhs, const common::Handshake& rhs) {
  EXPECT_STREQ(lhs.api_version, rhs.api_version);
  EXPECT_STREQ(lhs.application, rhs.application);
  EXPECT_STREQ(lhs.hostname, rhs.hostname);
  EXPECT_EQ(lhs.pid, rhs.pid);
  EXPECT_STREQ(lhs.uuid, rhs.uuid);
  EXPECT_STREQ(lhs.login, rhs.login);
  EXPECT_STREQ(lhs.password, rhs.password);
  // TODO(thraneh): subscriptions
}
void compare(const common::HandshakeAck& lhs, const common::HandshakeAck& rhs) {
  EXPECT_STREQ(lhs.api_version, rhs.api_version);
  EXPECT_STREQ(lhs.application, rhs.application);
  EXPECT_STREQ(lhs.hostname, rhs.hostname);
  EXPECT_EQ(lhs.pid, rhs.pid);
  EXPECT_EQ(lhs.failure, rhs.failure);
  EXPECT_STREQ(lhs.failure_reason, rhs.failure_reason);
  EXPECT_STREQ(lhs.server_uuid, rhs.server_uuid);
  EXPECT_STREQ(lhs.server_name, rhs.server_name);
  EXPECT_STREQ(lhs.shmem_s2c_name, rhs.shmem_s2c_name);
  EXPECT_EQ(lhs.shmem_s2c_size, rhs.shmem_s2c_size);
  EXPECT_STREQ(lhs.shmem_c2s_name, rhs.shmem_c2s_name);
  EXPECT_EQ(lhs.shmem_c2s_size, rhs.shmem_c2s_size);
}
void compare(const common::Heartbeat& lhs, const common::Heartbeat& rhs) {
  EXPECT_EQ(lhs.opaque, rhs.opaque);
}
void compare(const common::HeartbeatAck& lhs, const common::HeartbeatAck& rhs) {
  EXPECT_EQ(lhs.opaque, rhs.opaque);
}
void compare(const common::Ready& lhs, const common::Ready& rhs) {
}
void compare(const common::GatewayStatus& lhs, const common::GatewayStatus& rhs) {
  EXPECT_STREQ(lhs.name, rhs.name);
  EXPECT_EQ(lhs.status, rhs.status);
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
  compare(lhs.exchange_time, rhs.exchange_time);
  EXPECT_EQ(lhs.channel, rhs.channel);
}
void compare(const common::TradeSummary& lhs, const common::TradeSummary& rhs) {
  EXPECT_STREQ(lhs.exchange, rhs.exchange);
  EXPECT_STREQ(lhs.instrument, rhs.instrument);
  EXPECT_EQ(lhs.price, rhs.price);
  EXPECT_EQ(lhs.volume, rhs.volume);
  EXPECT_EQ(lhs.turnover, rhs.turnover);
  EXPECT_EQ(lhs.direction, rhs.direction);
  compare(lhs.exchange_time, rhs.exchange_time);
  EXPECT_EQ(lhs.channel, rhs.channel);
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
void compare(const common::ModifyOrder& lhs, const common::ModifyOrder& rhs) {
  EXPECT_EQ(lhs.order_id, rhs.order_id);
  EXPECT_EQ(lhs.quantity_change, rhs.quantity_change);
  EXPECT_EQ(lhs.limit_price, rhs.limit_price);
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
void compare(const common::CancelOrder& lhs, const common::CancelOrder& rhs) {
  EXPECT_EQ(lhs.order_id, rhs.order_id);
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
  compare(lhs.insert_time, rhs.insert_time);
  compare(lhs.cancel_time, rhs.cancel_time);
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
  compare(lhs.trade_time, rhs.trade_time);
}
void compare(const common::PositionUpdate& lhs, const common::PositionUpdate& rhs) {
  EXPECT_STREQ(lhs.exchange, rhs.exchange);
  EXPECT_STREQ(lhs.instrument, rhs.instrument);
  EXPECT_EQ(lhs.trade_direction, rhs.trade_direction);
  EXPECT_EQ(lhs.position, rhs.position);
  EXPECT_EQ(lhs.position_yesterday, rhs.position_yesterday);
  EXPECT_EQ(lhs.frozen_position, rhs.frozen_position);
  EXPECT_EQ(lhs.frozen_closing, rhs.frozen_closing);
  EXPECT_EQ(lhs.frozen_closing_yesterday, rhs.frozen_closing_yesterday);
}
}  // namespace

TEST(chrono, simple) {
  auto source = std::chrono::system_clock::now();
  auto tmp = common::time_point_to_uint64(source);
  auto target = common::uint64_to_time_point(tmp);
  compare(target, source);
}

TEST(flatbuffers, handshake_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // request (source)
    auto source_source_info = CreateRandomSourceInfo();
    auto source_handshake = CreateRandomHandshake();
    // serialize using flatbuffers
    fbb.Finish(common::convert2(
          fbb,
          source_source_info,
          source_handshake));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = common::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::Handshake);
    auto target_handshake = common::convert(root->event_data_as_Handshake());
    // validate
    compare(target_source_info, source_source_info);
    compare(target_handshake, source_handshake);
  }
}

TEST(flatbuffers, handshake_ack_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    auto source_source_info = CreateRandomSourceInfo();
    auto source_handshake_ack = CreateRandomHandshakeAck();
    // serialize using flatbuffers
    fbb.Finish(common::convert2(
          fbb,
          source_source_info,
          source_handshake_ack));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = common::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::HandshakeAck);
    auto target_handshake_ack = common::convert(root->event_data_as_HandshakeAck());
    // validate
    compare(target_source_info, source_source_info);
    compare(target_handshake_ack, source_handshake_ack);
  }
}

TEST(flatbuffers, heartbeat_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // request (source)
    auto source_source_info = CreateRandomSourceInfo();
    auto source_heartbeat = CreateRandomHeartbeat();
    // serialize using flatbuffers
    fbb.Finish(common::convert2(
          fbb,
          source_source_info,
          source_heartbeat));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = common::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::Heartbeat);
    auto target_heartbeat = common::convert(root->event_data_as_Heartbeat());
    // validate
    compare(target_source_info, source_source_info);
    compare(target_heartbeat, source_heartbeat);
  }
}

TEST(flatbuffers, heartbeat_ack_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    auto source_source_info = CreateRandomSourceInfo();
    auto source_heartbeat_ack = CreateRandomHeartbeatAck();
    // serialize using flatbuffers
    fbb.Finish(common::convert2(
          fbb,
          source_source_info,
          source_heartbeat_ack));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = common::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::HeartbeatAck);
    auto target_heartbeat_ack = common::convert(root->event_data_as_HeartbeatAck());
    // validate
    compare(target_source_info, source_source_info);
    compare(target_heartbeat_ack, source_heartbeat_ack);
  }
}

TEST(flatbuffers, ready_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    auto source_source_info = CreateRandomSourceInfo();
    auto source_ready = CreateRandomReady();
    // serialize using flatbuffers
    fbb.Finish(common::convert2(
          fbb,
          source_source_info,
          source_ready));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = common::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::Ready);
    auto target_ready = common::convert(root->event_data_as_Ready());
    // validate
    compare(target_source_info, source_source_info);
    compare(target_ready, source_ready);
  }
}

TEST(flatbuffers, gateway_status_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    auto source_source_info = CreateRandomSourceInfo();
    auto source_gateway_status = CreateRandomGatewayStatus();
    // serialize using flatbuffers
    fbb.Finish(common::convert2(
          fbb,
          source_source_info,
          source_gateway_status));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = common::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::GatewayStatus);
    auto target_gateway_status = common::convert(root->event_data_as_GatewayStatus());
    // validate
    compare(target_source_info, source_source_info);
    compare(target_gateway_status, source_gateway_status);
  }
}

TEST(flatbuffers, market_by_price_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    auto source_source_info = CreateRandomSourceInfo();
    auto source_market_by_price = CreateRandomMarketByPrice();
    // serialize using flatbuffers
    fbb.Finish(common::convert2(
          fbb,
          source_source_info,
          source_market_by_price));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = common::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::MarketByPrice);
    auto target_market_by_price = common::convert(root->event_data_as_MarketByPrice());
    // validate
    compare(target_source_info, source_source_info);
    compare(target_market_by_price, source_market_by_price);
  }
}

TEST(flatbuffers, trade_summary_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    auto source_source_info = CreateRandomSourceInfo();
    auto source_trade_summary = CreateRandomTradeSummary();
    // serialize using flatbuffers
    fbb.Finish(common::convert2(
          fbb,
          source_source_info,
          source_trade_summary));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = common::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::TradeSummary);
    auto target_trade_summary = common::convert(root->event_data_as_TradeSummary());
    // validate
    compare(target_source_info, source_source_info);
    compare(target_trade_summary, source_trade_summary);
  }
}

TEST(flatbuffers, reference_data_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    auto source_source_info = CreateRandomSourceInfo();
    auto source_reference_data = CreateRandomReferenceData();
    // serialize using flatbuffers
    fbb.Finish(common::convert2(
          fbb,
          source_source_info,
          source_reference_data));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = common::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::ReferenceData);
    auto target_reference_data = common::convert(root->event_data_as_ReferenceData());
    // validate
    compare(target_source_info, source_source_info);
    compare(target_reference_data, source_reference_data);
  }
}

TEST(flatbuffers, market_status_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    auto source_source_info = CreateRandomSourceInfo();
    auto source_market_status = CreateRandomMarketStatus();
    // serialize using flatbuffers
    fbb.Finish(common::convert2(
          fbb,
          source_source_info,
          source_market_status));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = common::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::MarketStatus);
    auto target_market_status = common::convert(root->event_data_as_MarketStatus());
    // validate
    compare(target_source_info, source_source_info);
    compare(target_market_status, source_market_status);
  }
}

TEST(flatbuffers, create_order_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // request (source)
    auto source_source_info = CreateRandomSourceInfo();
    auto source_create_order = CreateRandomCreateOrder();
    // serialize using flatbuffers
    fbb.Finish(common::convert2(
          fbb,
          source_source_info,
          source_create_order));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = common::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::CreateOrder);
    auto target_create_order = common::convert(root->event_data_as_CreateOrder());
    // validate
    compare(target_source_info, source_source_info);
    compare(target_create_order, source_create_order);
  }
}

TEST(flatbuffers, create_order_ack_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    auto source_source_info = CreateRandomSourceInfo();
    auto source_create_order_ack = CreateRandomCreateOrderAck();
    // serialize using flatbuffers
    fbb.Finish(common::convert2(
          fbb,
          source_source_info,
          source_create_order_ack));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = common::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::CreateOrderAck);
    auto target_create_order_ack = common::convert(root->event_data_as_CreateOrderAck());
    // validate
    compare(target_source_info, source_source_info);
    compare(target_create_order_ack, source_create_order_ack);
  }
}

TEST(flatbuffers, modify_order_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // request (source)
    auto source_source_info = CreateRandomSourceInfo();
    auto source_modify_order = CreateRandomModifyOrder();
    // serialize using flatbuffers
    fbb.Finish(common::convert2(
          fbb,
          source_source_info,
          source_modify_order));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = common::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::ModifyOrder);
    auto target_modify_order = common::convert(root->event_data_as_ModifyOrder());
    // validate
    compare(target_source_info, source_source_info);
    compare(target_modify_order, source_modify_order);
  }
}

TEST(flatbuffers, modify_order_ack_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    auto source_source_info = CreateRandomSourceInfo();
    auto source_modify_order_ack = CreateRandomModifyOrderAck();
    // serialize using flatbuffers
    fbb.Finish(common::convert2(
          fbb,
          source_source_info,
          source_modify_order_ack));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = common::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::ModifyOrderAck);
    auto target_modify_order_ack = common::convert(root->event_data_as_ModifyOrderAck());
    // validate
    compare(target_source_info, source_source_info);
    compare(target_modify_order_ack, source_modify_order_ack);
  }
}

TEST(flatbuffers, cancel_order_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // request (source)
    auto source_source_info = CreateRandomSourceInfo();
    auto source_cancel_order = CreateRandomCancelOrder();
    // serialize using flatbuffers
    fbb.Finish(common::convert2(
          fbb,
          source_source_info,
          source_cancel_order));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = common::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::CancelOrder);
    auto target_cancel_order = common::convert(root->event_data_as_CancelOrder());
    // validate
    compare(target_source_info, source_source_info);
    compare(target_cancel_order, source_cancel_order);
  }
}

TEST(flatbuffers, cancel_order_ack_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    auto source_source_info = CreateRandomSourceInfo();
    auto source_cancel_order_ack = CreateRandomCancelOrderAck();
    // serialize using flatbuffers
    fbb.Finish(common::convert2(
          fbb,
          source_source_info,
          source_cancel_order_ack));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = common::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::CancelOrderAck);
    auto target_cancel_order_ack = common::convert(root->event_data_as_CancelOrderAck());
    // validate
    compare(target_source_info, source_source_info);
    compare(target_cancel_order_ack, source_cancel_order_ack);
  }
}

TEST(flatbuffers, order_update_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    auto source_source_info = CreateRandomSourceInfo();
    auto source_order_update = CreateRandomOrderUpdate();
    // serialize using flatbuffers
    fbb.Finish(common::convert2(
          fbb,
          source_source_info,
          source_order_update));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = common::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::OrderUpdate);
    auto target_order_update = common::convert(root->event_data_as_OrderUpdate());
    // validate
    compare(target_source_info, source_source_info);
    compare(target_order_update, source_order_update);
  }
}

TEST(flatbuffers, trade_update_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    auto source_source_info = CreateRandomSourceInfo();
    auto source_trade_update = CreateRandomTradeUpdate();
    // serialize using flatbuffers
    fbb.Finish(common::convert2(
          fbb,
          source_source_info,
          source_trade_update));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = common::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::TradeUpdate);
    auto target_trade_update = common::convert(root->event_data_as_TradeUpdate());
    // validate
    compare(target_source_info, source_source_info);
    compare(target_trade_update, source_trade_update);
  }
}

TEST(flatbuffers, position_update_event) {
  FlatBufferBuilder fbb;
  for (auto i = 0; i < MAX_ITERATIONS; ++i) {
    // reset
    fbb.Clear();  // doesn't de-allocate
    EXPECT_EQ(fbb.GetSize(), 0);
    // event (source)
    auto source_source_info = CreateRandomSourceInfo();
    auto source_position_update = CreateRandomPositionUpdate();
    // serialize using flatbuffers
    fbb.Finish(common::convert2(
          fbb,
          source_source_info,
          source_position_update));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = common::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::PositionUpdate);
    auto target_position_update = common::convert(root->event_data_as_PositionUpdate());
    // validate
    compare(target_source_info, source_source_info);
    compare(target_position_update, source_position_update);
  }
}
