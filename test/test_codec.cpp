/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <gtest/gtest.h>
#include <cstdlib>
#include <random>
#include "roq/codec.h"

using namespace flatbuffers;  // NOLINT
using namespace roq;  // NOLINT

const int MAX_ITERATIONS = 1000;

namespace {
static std::random_device random_device;
static std::mt19937_64 random_engine(random_device());
static std::uniform_int_distribution<uint16_t> random_generator_uint16;
static uint16_t rand_uint16() {
  return random_generator_uint16(random_engine);
}
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
static time_point_t rand_time_point() {
  const auto microseconds = random_generator_uint64(random_engine);
  const auto duration = std::chrono::microseconds(microseconds);
  return time_point_t(duration);
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
static GatewayState rand_gateway_state() {
  return static_cast<GatewayState>(rand_uint32() % static_cast<uint32_t>(GatewayState::MAX));
}
static Side rand_side() {
  return static_cast<Side>(rand_uint32() % static_cast<uint32_t>(Side::MAX));
}
static OrderType rand_order_type() {
  return static_cast<OrderType>(rand_uint32() % static_cast<uint32_t>(OrderType::MAX));
}
static TimeInForce rand_time_in_force() {
  return static_cast<TimeInForce>(rand_uint32() % static_cast<uint32_t>(TimeInForce::MAX));
}
static PositionEffect rand_position_effect() {
  return static_cast<PositionEffect>(rand_uint32() % static_cast<uint32_t>(PositionEffect::MAX));
}
static TradingStatus rand_trading_status() {
  return static_cast<TradingStatus>(rand_uint32() % static_cast<uint32_t>(TradingStatus::MAX));
}
static OrderStatus rand_order_status() {
  return static_cast<OrderStatus>(rand_uint32() % static_cast<uint32_t>(OrderStatus::MAX));
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
inline SourceInfo CreateRandomSourceInfo() {
  return SourceInfo {
    .seqno = rand_uint64(),
    .create_time = rand_time_point()
  };
}
inline Handshake CreateRandomHandshake() {
  return Handshake {
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
inline HandshakeAck CreateRandomHandshakeAck() {
  return HandshakeAck {
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
inline Heartbeat CreateRandomHeartbeat() {
  return Heartbeat {
    .opaque = rand_int64(),
  };
}
inline HeartbeatAck CreateRandomHeartbeatAck() {
  return HeartbeatAck {
    .opaque = rand_int64(),
  };
}
inline DownloadBegin CreateRandomDownloadBegin() {
  return DownloadBegin {
  };
}
inline DownloadEnd CreateRandomDownloadEnd() {
  return DownloadEnd {
    .max_order_id = rand_uint32(),
  };
}
inline GatewayStatus CreateRandomGatewayStatus() {
  return GatewayStatus {
    .name = NAME[rand_uint32() % NAME_LENGTH],
    .status = rand_gateway_state(),
  };
}
inline MarketByPrice CreateRandomMarketByPrice() {
  MarketByPrice res {
    .exchange = NAME[rand_uint32() % NAME_LENGTH],
    .symbol = NAME[rand_uint32() % NAME_LENGTH],
  };
  for (auto i = 0; i < MAX_DEPTH; ++i) {
    res.depth[i].bid_price = rand_double();
    res.depth[i].bid_quantity = rand_double();
    res.depth[i].ask_price = rand_double();
    res.depth[i].ask_quantity = rand_double();
  }
  res.exchange_time = rand_time_point();
  res.channel = rand_uint16();
  return res;
}
inline TradeSummary CreateRandomTradeSummary() {
  return TradeSummary {
    .exchange = NAME[rand_uint32() % NAME_LENGTH],
    .symbol = NAME[rand_uint32() % NAME_LENGTH],
    .price = rand_double(),
    .volume = rand_double(),
    .turnover = rand_double(),
    .side = rand_side(),
    .exchange_time = rand_time_point(),
    .channel = rand_uint16(),
  };
}
inline ReferenceData CreateRandomReferenceData() {
  return ReferenceData {
    .exchange = NAME[rand_uint32() % NAME_LENGTH],
    .symbol = NAME[rand_uint32() % NAME_LENGTH],
    .tick_size = rand_double(),
    .limit_up = rand_double(),
    .limit_down = rand_double(),
  };
}
inline MarketStatus CreateRandomMarketStatus() {
  return MarketStatus {
    .exchange = NAME[rand_uint32() % NAME_LENGTH],
    .symbol = NAME[rand_uint32() % NAME_LENGTH],
    .trading_status = rand_trading_status(),
  };
}
inline PositionUpdate CreateRandomPositionUpdate() {
  return PositionUpdate {
    .account = NAME[rand_uint32() % NAME_LENGTH],
    .exchange = NAME[rand_uint32() % NAME_LENGTH],
    .symbol = NAME[rand_uint32() % NAME_LENGTH],
    .side = rand_side(),
    .position = rand_double(),
    .yesterday = rand_double(),
    .last_order_id = rand_uint32(),
    .last_trade_id = rand_uint32(),
  };
}
inline OrderUpdate CreateRandomOrderUpdate() {
  return OrderUpdate {
    .order_id = rand_uint32(),
    .account = NAME[rand_uint32() % NAME_LENGTH],
    .exchange = NAME[rand_uint32() % NAME_LENGTH],
    .symbol = NAME[rand_uint32() % NAME_LENGTH],
    .order_status = rand_order_status(),
    .side = rand_side(),
    .remaining_quantity = rand_double(),
    .traded_quantity = rand_double(),
    .position_effect = rand_position_effect(),
    .order_template = NAME[rand_uint32() % NAME_LENGTH],
    .insert_time = rand_time_point(),
    .cancel_time = rand_time_point(),
    .order_local_id = rand_uint32(),
    .order_external_id = NAME[rand_uint32() % NAME_LENGTH],
  };
}
inline TradeUpdate CreateRandomTradeUpdate() {
  return TradeUpdate {
    .trade_id = rand_uint32(),
    .order_id = rand_uint32(),
    .account = NAME[rand_uint32() % NAME_LENGTH],
    .exchange = NAME[rand_uint32() % NAME_LENGTH],
    .symbol = NAME[rand_uint32() % NAME_LENGTH],
    .side = rand_side(),
    .quantity = rand_double(),
    .price = rand_double(),
    .position_effect = rand_position_effect(),
    .order_template = NAME[rand_uint32() % NAME_LENGTH],
    .trade_time = rand_time_point(),
    .order_local_id = rand_uint32(),
    .order_external_id = NAME[rand_uint32() % NAME_LENGTH],
    .trade_external_id = NAME[rand_uint32() % NAME_LENGTH],
  };
}
inline CreateOrder CreateRandomCreateOrder() {
  return CreateOrder {
    .order_id = rand_uint32(),
    .account = NAME[rand_uint32() % NAME_LENGTH],
    .exchange = NAME[rand_uint32() % NAME_LENGTH],
    .symbol = NAME[rand_uint32() % NAME_LENGTH],
    .side = rand_side(),
    .quantity = rand_double(),
    .order_type = rand_order_type(),
    .limit_price = rand_double(),
    .time_in_force = rand_time_in_force(),
    .position_effect = rand_position_effect(),
    .order_template = NAME[rand_uint32() % NAME_LENGTH],
  };
}
inline ModifyOrder CreateRandomModifyOrder() {
  return ModifyOrder {
    .order_id = rand_uint32(),
    .quantity_change = rand_double(),
    .limit_price = rand_double(),
  };
}
inline CancelOrder CreateRandomCancelOrder() {
  return CancelOrder {
    .order_id = rand_uint32(),
  };
}
inline CreateOrderAck CreateRandomCreateOrderAck() {
  return CreateOrderAck {
    .order_id = rand_uint32(),
    .failure = rand_bool(),
    .reason = NAME[rand_uint32() % NAME_LENGTH],
    .order_local_id = rand_uint32(),
    .order_external_id = NAME[rand_uint32() % NAME_LENGTH],
  };
}
inline ModifyOrderAck CreateRandomModifyOrderAck() {
  return ModifyOrderAck {
    .order_id = rand_uint32(),
    .failure = rand_bool(),
    .reason = NAME[rand_uint32() % NAME_LENGTH],
    .order_local_id = rand_uint32(),
    .order_external_id = NAME[rand_uint32() % NAME_LENGTH],
  };
}
inline CancelOrderAck CreateRandomCancelOrderAck() {
  return CancelOrderAck {
    .order_id = rand_uint32(),
    .failure = rand_bool(),
    .reason = NAME[rand_uint32() % NAME_LENGTH],
    .order_local_id = rand_uint32(),
    .order_external_id = NAME[rand_uint32() % NAME_LENGTH],
  };
}
}  // namespace

namespace {
// comparing time-points must go through normalisation
// for example, if we use system_clock::now(), we may have a higher
// resolution than the one we use on-the-wire, in other words, we
// may truncate a time-point when we encode it
void compare(time_point_t lhs, time_point_t rhs) {
  auto lhs_adj = codec::uint64_to_time_point(
      codec::time_point_to_uint64(lhs));
  auto rhs_adj = codec::uint64_to_time_point(
      codec::time_point_to_uint64(rhs));
  EXPECT_EQ(lhs_adj, rhs_adj);
}
void compare(const SourceInfo& lhs, const SourceInfo& rhs) {
  EXPECT_EQ(lhs.seqno, rhs.seqno);
  compare(lhs.create_time, rhs.create_time);
}
void compare(const Handshake& lhs, const Handshake& rhs) {
  EXPECT_STREQ(lhs.api_version, rhs.api_version);
  EXPECT_STREQ(lhs.application, rhs.application);
  EXPECT_STREQ(lhs.hostname, rhs.hostname);
  EXPECT_EQ(lhs.pid, rhs.pid);
  EXPECT_STREQ(lhs.uuid, rhs.uuid);
  EXPECT_STREQ(lhs.login, rhs.login);
  EXPECT_STREQ(lhs.password, rhs.password);
  // TODO(thraneh): subscriptions
}
void compare(const HandshakeAck& lhs, const HandshakeAck& rhs) {
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
void compare(const Heartbeat& lhs, const Heartbeat& rhs) {
  EXPECT_EQ(lhs.opaque, rhs.opaque);
}
void compare(const HeartbeatAck& lhs, const HeartbeatAck& rhs) {
  EXPECT_EQ(lhs.opaque, rhs.opaque);
}
void compare(const DownloadEnd& lhs, const DownloadEnd& rhs) {
  EXPECT_EQ(lhs.max_order_id, rhs.max_order_id);
}
void compare(const GatewayStatus& lhs, const GatewayStatus& rhs) {
  EXPECT_STREQ(lhs.name, rhs.name);
  EXPECT_EQ(lhs.status, rhs.status);
}
void compare(const MarketByPrice& lhs, const MarketByPrice& rhs) {
  EXPECT_STREQ(lhs.exchange, rhs.exchange);
  EXPECT_STREQ(lhs.symbol, rhs.symbol);
  for (auto i = 0; i < MAX_DEPTH; ++i) {
    EXPECT_EQ(lhs.depth[i].bid_price, rhs.depth[i].bid_price);
    EXPECT_EQ(lhs.depth[i].bid_quantity, rhs.depth[i].bid_quantity);
    EXPECT_EQ(lhs.depth[i].ask_price, rhs.depth[i].ask_price);
    EXPECT_EQ(lhs.depth[i].ask_quantity, rhs.depth[i].ask_quantity);
  }
  compare(lhs.exchange_time, rhs.exchange_time);
  EXPECT_EQ(lhs.channel, rhs.channel);
}
void compare(const TradeSummary& lhs, const TradeSummary& rhs) {
  EXPECT_STREQ(lhs.exchange, rhs.exchange);
  EXPECT_STREQ(lhs.symbol, rhs.symbol);
  EXPECT_EQ(lhs.price, rhs.price);
  EXPECT_EQ(lhs.volume, rhs.volume);
  EXPECT_EQ(lhs.turnover, rhs.turnover);
  EXPECT_EQ(lhs.side, rhs.side);
  compare(lhs.exchange_time, rhs.exchange_time);
  EXPECT_EQ(lhs.channel, rhs.channel);
}
void compare(const ReferenceData& lhs, const ReferenceData& rhs) {
  EXPECT_STREQ(lhs.exchange, rhs.exchange);
  EXPECT_STREQ(lhs.symbol, rhs.symbol);
  EXPECT_EQ(lhs.tick_size, rhs.tick_size);
  EXPECT_EQ(lhs.limit_up, rhs.limit_up);
  EXPECT_EQ(lhs.limit_down, rhs.limit_down);
}
void compare(const MarketStatus& lhs, const MarketStatus& rhs) {
  EXPECT_STREQ(lhs.exchange, rhs.exchange);
  EXPECT_STREQ(lhs.symbol, rhs.symbol);
  EXPECT_EQ(lhs.trading_status, rhs.trading_status);
}
void compare(const PositionUpdate& lhs, const PositionUpdate& rhs) {
  EXPECT_STREQ(lhs.account, rhs.account);
  EXPECT_STREQ(lhs.exchange, rhs.exchange);
  EXPECT_STREQ(lhs.symbol, rhs.symbol);
  EXPECT_EQ(lhs.side, rhs.side);
  EXPECT_EQ(lhs.position, rhs.position);
  EXPECT_EQ(lhs.yesterday, rhs.yesterday);
  EXPECT_EQ(lhs.last_order_id, rhs.last_order_id);
  EXPECT_EQ(lhs.last_trade_id, rhs.last_trade_id);
}
void compare(const OrderUpdate& lhs, const OrderUpdate& rhs) {
  EXPECT_EQ(lhs.order_id, rhs.order_id);
  EXPECT_STREQ(lhs.account, rhs.account);
  EXPECT_STREQ(lhs.exchange, rhs.exchange);
  EXPECT_STREQ(lhs.symbol, rhs.symbol);
  EXPECT_EQ(lhs.order_status, rhs.order_status);
  EXPECT_EQ(lhs.side, rhs.side);
  EXPECT_EQ(lhs.remaining_quantity, rhs.remaining_quantity);
  EXPECT_EQ(lhs.traded_quantity, rhs.traded_quantity);
  EXPECT_EQ(lhs.position_effect, rhs.position_effect);
  EXPECT_STREQ(lhs.order_template, rhs.order_template);
  compare(lhs.insert_time, rhs.insert_time);
  compare(lhs.cancel_time, rhs.cancel_time);
  EXPECT_EQ(lhs.order_local_id, rhs.order_local_id);
  EXPECT_STREQ(lhs.order_external_id, rhs.order_external_id);
}
void compare(const TradeUpdate& lhs, const TradeUpdate& rhs) {
  EXPECT_EQ(lhs.trade_id, rhs.trade_id);
  EXPECT_EQ(lhs.order_id, rhs.order_id);
  EXPECT_STREQ(lhs.account, rhs.account);
  EXPECT_STREQ(lhs.exchange, rhs.exchange);
  EXPECT_STREQ(lhs.symbol, rhs.symbol);
  EXPECT_EQ(lhs.side, rhs.side);
  EXPECT_EQ(lhs.quantity, rhs.quantity);
  EXPECT_EQ(lhs.price, rhs.price);
  EXPECT_EQ(lhs.position_effect, rhs.position_effect);
  EXPECT_STREQ(lhs.order_template, rhs.order_template);
  compare(lhs.trade_time, rhs.trade_time);
  EXPECT_EQ(lhs.order_local_id, rhs.order_local_id);
  EXPECT_STREQ(lhs.order_external_id, rhs.order_external_id);
  EXPECT_STREQ(lhs.trade_external_id, rhs.trade_external_id);
}
void compare(const CreateOrder& lhs, const CreateOrder& rhs) {
  EXPECT_EQ(lhs.order_id, rhs.order_id);
  EXPECT_STREQ(lhs.account, rhs.account);
  EXPECT_STREQ(lhs.exchange, rhs.exchange);
  EXPECT_STREQ(lhs.symbol, rhs.symbol);
  EXPECT_EQ(lhs.side, rhs.side);
  EXPECT_EQ(lhs.quantity, rhs.quantity);
  EXPECT_EQ(lhs.order_type, rhs.order_type);
  EXPECT_EQ(lhs.limit_price, rhs.limit_price);
  EXPECT_EQ(lhs.time_in_force, rhs.time_in_force);
  EXPECT_EQ(lhs.position_effect, rhs.position_effect);
  EXPECT_STREQ(lhs.order_template, rhs.order_template);
}
void compare(const ModifyOrder& lhs, const ModifyOrder& rhs) {
  EXPECT_EQ(lhs.order_id, rhs.order_id);
  EXPECT_EQ(lhs.quantity_change, rhs.quantity_change);
  EXPECT_EQ(lhs.limit_price, rhs.limit_price);
}
void compare(const CancelOrder& lhs, const CancelOrder& rhs) {
  EXPECT_EQ(lhs.order_id, rhs.order_id);
}
void compare(const CreateOrderAck& lhs, const CreateOrderAck& rhs) {
  EXPECT_EQ(lhs.order_id, rhs.order_id);
  EXPECT_EQ(lhs.failure, rhs.failure);
  EXPECT_STREQ(lhs.reason, rhs.reason);
  EXPECT_EQ(lhs.order_local_id, rhs.order_local_id);
  EXPECT_STREQ(lhs.order_external_id, rhs.order_external_id);
}
void compare(const ModifyOrderAck& lhs, const ModifyOrderAck& rhs) {
  EXPECT_EQ(lhs.order_id, rhs.order_id);
  EXPECT_EQ(lhs.failure, rhs.failure);
  EXPECT_STREQ(lhs.reason, rhs.reason);
  EXPECT_EQ(lhs.order_local_id, rhs.order_local_id);
  EXPECT_STREQ(lhs.order_external_id, rhs.order_external_id);
}
void compare(const CancelOrderAck& lhs, const CancelOrderAck& rhs) {
  EXPECT_EQ(lhs.order_id, rhs.order_id);
  EXPECT_EQ(lhs.failure, rhs.failure);
  EXPECT_STREQ(lhs.reason, rhs.reason);
  EXPECT_EQ(lhs.order_local_id, rhs.order_local_id);
  EXPECT_STREQ(lhs.order_external_id, rhs.order_external_id);
}
}  // namespace

TEST(chrono, simple) {
  auto source = std::chrono::system_clock::now();
  auto tmp = codec::time_point_to_uint64(source);
  auto target = codec::uint64_to_time_point(tmp);
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
    fbb.Finish(codec::convert2(
          fbb,
          source_source_info,
          source_handshake));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = codec::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::Handshake);
    auto target_handshake = codec::convert(root->event_data_as_Handshake());
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
    fbb.Finish(codec::convert2(
          fbb,
          source_source_info,
          source_handshake_ack));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = codec::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::HandshakeAck);
    auto target_handshake_ack = codec::convert(root->event_data_as_HandshakeAck());
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
    fbb.Finish(codec::convert2(
          fbb,
          source_source_info,
          source_heartbeat));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = codec::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::Heartbeat);
    auto target_heartbeat = codec::convert(root->event_data_as_Heartbeat());
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
    fbb.Finish(codec::convert2(
          fbb,
          source_source_info,
          source_heartbeat_ack));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = codec::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::HeartbeatAck);
    auto target_heartbeat_ack = codec::convert(root->event_data_as_HeartbeatAck());
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
    auto source_ready = CreateRandomDownloadEnd();
    // serialize using flatbuffers
    fbb.Finish(codec::convert2(
          fbb,
          source_source_info,
          source_ready));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = codec::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::DownloadEnd);
    auto target_ready = codec::convert(root->event_data_as_DownloadEnd());
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
    fbb.Finish(codec::convert2(
          fbb,
          source_source_info,
          source_gateway_status));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = codec::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::GatewayStatus);
    auto target_gateway_status = codec::convert(root->event_data_as_GatewayStatus());
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
    fbb.Finish(codec::convert2(
          fbb,
          source_source_info,
          source_market_by_price));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = codec::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::MarketByPrice);
    auto target_market_by_price = codec::convert(root->event_data_as_MarketByPrice());
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
    fbb.Finish(codec::convert2(
          fbb,
          source_source_info,
          source_trade_summary));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = codec::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::TradeSummary);
    auto target_trade_summary = codec::convert(root->event_data_as_TradeSummary());
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
    fbb.Finish(codec::convert2(
          fbb,
          source_source_info,
          source_reference_data));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = codec::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::ReferenceData);
    auto target_reference_data = codec::convert(root->event_data_as_ReferenceData());
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
    fbb.Finish(codec::convert2(
          fbb,
          source_source_info,
          source_market_status));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = codec::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::MarketStatus);
    auto target_market_status = codec::convert(root->event_data_as_MarketStatus());
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
    fbb.Finish(codec::convert2(
          fbb,
          source_source_info,
          source_create_order));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = codec::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::CreateOrder);
    auto target_create_order = codec::convert(root->event_data_as_CreateOrder());
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
    fbb.Finish(codec::convert2(
          fbb,
          source_source_info,
          source_create_order_ack));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = codec::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::CreateOrderAck);
    auto target_create_order_ack = codec::convert(root->event_data_as_CreateOrderAck());
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
    fbb.Finish(codec::convert2(
          fbb,
          source_source_info,
          source_modify_order));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = codec::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::ModifyOrder);
    auto target_modify_order = codec::convert(root->event_data_as_ModifyOrder());
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
    fbb.Finish(codec::convert2(
          fbb,
          source_source_info,
          source_modify_order_ack));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = codec::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::ModifyOrderAck);
    auto target_modify_order_ack = codec::convert(root->event_data_as_ModifyOrderAck());
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
    fbb.Finish(codec::convert2(
          fbb,
          source_source_info,
          source_cancel_order));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = codec::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::CancelOrder);
    auto target_cancel_order = codec::convert(root->event_data_as_CancelOrder());
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
    fbb.Finish(codec::convert2(
          fbb,
          source_source_info,
          source_cancel_order_ack));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = codec::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::CancelOrderAck);
    auto target_cancel_order_ack = codec::convert(root->event_data_as_CancelOrderAck());
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
    fbb.Finish(codec::convert2(
          fbb,
          source_source_info,
          source_order_update));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = codec::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::OrderUpdate);
    auto target_order_update = codec::convert(root->event_data_as_OrderUpdate());
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
    fbb.Finish(codec::convert2(
          fbb,
          source_source_info,
          source_trade_update));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = codec::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::TradeUpdate);
    auto target_trade_update = codec::convert(root->event_data_as_TradeUpdate());
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
    fbb.Finish(codec::convert2(
          fbb,
          source_source_info,
          source_position_update));
    EXPECT_GT(fbb.GetSize(), 0);
    // copy of the buffer (just making sure...)
    std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
    // deserialize using flatbuffers
    auto root = GetRoot<schema::Event>(&buffer[0]);
    auto target_source_info = codec::convert(root->source_info());
    EXPECT_EQ(root->event_data_type(), schema::EventData::PositionUpdate);
    auto target_position_update = codec::convert(root->event_data_as_PositionUpdate());
    // validate
    compare(target_source_info, source_source_info);
    compare(target_position_update, source_position_update);
  }
}
