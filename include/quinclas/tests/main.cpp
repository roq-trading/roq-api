/* Copyright (c) 2017,  Hans Erik Thrane */

#include <cstdlib>
#include <random>
#include <gtest/gtest.h>
#include "quinclas/codec.h"

using namespace flatbuffers;
using namespace quinclas;

const int MAX_ITERATIONS = 1000;


// TODO: randomized enums

namespace {
static std::random_device random_device;
static std::mt19937_64 random_engine(random_device());
static std::uniform_int_distribution<uint32_t> random_generator_uint32;
static uint32_t rand_uint32() {
    return random_generator_uint32(random_engine);
}
static std::uniform_int_distribution<uint64_t> random_generator_uint64;
static uint32_t rand_uint64() {
    return random_generator_uint64(random_engine);
}
static std::uniform_int_distribution<double> random_generator_double;
static double rand_double() {
    return random_generator_double(random_engine);
}
static std::uniform_int_distribution<int> random_generator_int32;
static int32_t rand_int32() {
    return random_generator_int32(random_engine);
}
}  // namespace

namespace {
static const char * const NAME[] = {"G1", "Gateway2", "SomeVeryLongGatewayName3"};
static const size_t NAME_LENGTH = sizeof(NAME) / sizeof(NAME[0]);
}  // namespace

namespace {
inline common::MessageInfo CreateRandomMessageInfo() {
    return common::MessageInfo{
        .gateway = NAME[rand_uint32() % NAME_LENGTH],
        .message_id = rand_uint32(),
        .exchange_time = rand_uint64(),
        .receive_time = rand_uint64(),
        .enqueue_time = rand_uint64(),
    };
}
inline common::GatewayStatus CreateRandomGatewayStatus() {
    return common::GatewayStatus{
        .market_data_connection_status = common::ConnectionStatus::Connected,
        .market_data_login_status = common::LoginStatus::On,
        .order_management_connection_status = common::ConnectionStatus::Connected,
        .order_management_login_status = common::LoginStatus::On,
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
inline common::SessionStatistics CreateRandomSessionStatistics() {
    return common::SessionStatistics{
        .exchange = NAME[rand_uint32() % NAME_LENGTH],
        .instrument = NAME[rand_uint32() % NAME_LENGTH],
        .open = rand_double(),
        .high = rand_double(),
        .low = rand_double(),
    };
}
inline common::DailyStatistics CreateRandomDailyStatistics() {
    return common::DailyStatistics{
        .exchange = NAME[rand_uint32() % NAME_LENGTH],
        .instrument = NAME[rand_uint32() % NAME_LENGTH],
        .settlement = rand_double(),
        .open_interest = rand_double(),
        .volume = rand_double(),
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
        .trading_status = common::TradingStatus::Open,
    };
}
inline common::CreateOrderAck CreateRandomCreateOrderAck() {
    return common::CreateOrderAck{
        .opaque = rand_int32(),
        .order_id = NAME[rand_uint32() % NAME_LENGTH],
    };
}
}  // namespace

namespace {
void compare(const common::MessageInfo& lhs, const common::MessageInfo& rhs) {
    ASSERT_STREQ(lhs.gateway, rhs.gateway);
    ASSERT_EQ(lhs.message_id, rhs.message_id);
    ASSERT_EQ(lhs.exchange_time, rhs.exchange_time);
    ASSERT_EQ(lhs.receive_time, rhs.receive_time);
    ASSERT_EQ(lhs.enqueue_time, rhs.enqueue_time);
}
void compare(const common::GatewayStatus& lhs, const common::GatewayStatus& rhs) {
    ASSERT_EQ(lhs.market_data_connection_status, rhs.market_data_connection_status);
    ASSERT_EQ(lhs.market_data_login_status, rhs.market_data_login_status);
    ASSERT_EQ(lhs.order_management_connection_status, rhs.order_management_connection_status);
    ASSERT_EQ(lhs.order_management_login_status, rhs.order_management_login_status);
}
void compare(const common::MarketByPrice& lhs, const common::MarketByPrice& rhs) {
    ASSERT_STREQ(lhs.exchange, rhs.exchange);
    ASSERT_STREQ(lhs.instrument, rhs.instrument);
    for (auto i = 0; i < common::MAX_DEPTH; ++i) {
        ASSERT_EQ(lhs.depth[i].bid_price, rhs.depth[i].bid_price);
        ASSERT_EQ(lhs.depth[i].bid_quantity, rhs.depth[i].bid_quantity);
        ASSERT_EQ(lhs.depth[i].ask_price, rhs.depth[i].ask_price);
        ASSERT_EQ(lhs.depth[i].ask_quantity, rhs.depth[i].ask_quantity);
    }
}
void compare(const common::SessionStatistics& lhs, const common::SessionStatistics& rhs) {
    ASSERT_STREQ(lhs.exchange, rhs.exchange);
    ASSERT_STREQ(lhs.instrument, rhs.instrument);
    ASSERT_EQ(lhs.open, rhs.open);
    ASSERT_EQ(lhs.high, rhs.high);
    ASSERT_EQ(lhs.low, rhs.low);
}
void compare(const common::DailyStatistics& lhs, const common::DailyStatistics& rhs) {
    ASSERT_STREQ(lhs.exchange, rhs.exchange);
    ASSERT_STREQ(lhs.instrument, rhs.instrument);
    ASSERT_EQ(lhs.settlement, rhs.settlement);
    ASSERT_EQ(lhs.open_interest, rhs.open_interest);
    ASSERT_EQ(lhs.volume, rhs.volume);
}
void compare(const common::ReferenceData& lhs, const common::ReferenceData& rhs) {
    ASSERT_STREQ(lhs.exchange, rhs.exchange);
    ASSERT_STREQ(lhs.instrument, rhs.instrument);
    ASSERT_EQ(lhs.tick_size, rhs.tick_size);
    ASSERT_EQ(lhs.limit_up, rhs.limit_up);
    ASSERT_EQ(lhs.limit_down, rhs.limit_down);
}
void compare(const common::MarketStatus& lhs, const common::MarketStatus& rhs) {
    ASSERT_STREQ(lhs.exchange, rhs.exchange);
    ASSERT_STREQ(lhs.instrument, rhs.instrument);
    ASSERT_EQ(lhs.trading_status, rhs.trading_status);
}
void compare(const common::CreateOrderAck& lhs, const common::CreateOrderAck& rhs) {
    ASSERT_EQ(lhs.opaque, rhs.opaque);
    ASSERT_STREQ(lhs.order_id, rhs.order_id);
}
}  // namespace

TEST(flatbuffers, gateway_status_event) {
    FlatBufferBuilder fbb;
    for (auto i = 0; i < MAX_ITERATIONS; ++i) {
        // reset
        fbb.Clear();  // doesn't de-allocate
        ASSERT_EQ(fbb.GetSize(), 0);
        // event (source)
        const auto source = common::GatewayStatusEvent{
            .message_info = CreateRandomMessageInfo(),
            .gateway_status = CreateRandomGatewayStatus()};
        // serialize using flatbuffers
        fbb.Finish(common::convert(fbb, source));
        ASSERT_GT(fbb.GetSize(), 0);
        // copy of the buffer (just making sure...)
        const std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
        // deserialize using flatbuffers
        auto root = GetRoot<schema::Event>(&buffer[0]);
        auto message_info = common::convert(root->message_info());
        ASSERT_EQ(root->event_data_type(), schema::EventData::GatewayStatus);
        auto gateway_status = common::convert(root->event_data_as_GatewayStatus());
        // event (target)
        auto target = common::GatewayStatusEvent{
            .message_info = message_info,
            .gateway_status = gateway_status};
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
        ASSERT_EQ(fbb.GetSize(), 0);
        // event (source)
        const auto source = common::MarketByPriceEvent{
            .message_info = CreateRandomMessageInfo(),
            .market_by_price = CreateRandomMarketByPrice()};
        // serialize using flatbuffers
        fbb.Finish(common::convert(fbb, source));
        ASSERT_GT(fbb.GetSize(), 0);
        // copy of the buffer (just making sure...)
        const std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
        // deserialize using flatbuffers
        auto root = GetRoot<schema::Event>(&buffer[0]);
        auto message_info = common::convert(root->message_info());
        ASSERT_EQ(root->event_data_type(), schema::EventData::MarketByPrice);
        auto market_by_price = common::convert(root->event_data_as_MarketByPrice());
        // event (target)
        auto target = common::MarketByPriceEvent{
            .message_info = message_info,
            .market_by_price = market_by_price};
        // validate
        compare(target.message_info, source.message_info);
        compare(target.market_by_price, source.market_by_price);
    }
}

TEST(flatbuffers, session_statistics_event) {
    FlatBufferBuilder fbb;
    for (auto i = 0; i < MAX_ITERATIONS; ++i) {
        // reset
        fbb.Clear();  // doesn't de-allocate
        ASSERT_EQ(fbb.GetSize(), 0);
        // event (source)
        const auto source = common::SessionStatisticsEvent{
            .message_info = CreateRandomMessageInfo(),
            .session_statistics = CreateRandomSessionStatistics()};
        // serialize using flatbuffers
        fbb.Finish(common::convert(fbb, source));
        ASSERT_GT(fbb.GetSize(), 0);
        // copy of the buffer (just making sure...)
        const std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
        // deserialize using flatbuffers
        auto root = GetRoot<schema::Event>(&buffer[0]);
        auto message_info = common::convert(root->message_info());
        ASSERT_EQ(root->event_data_type(), schema::EventData::SessionStatistics);
        auto session_statistics = common::convert(root->event_data_as_SessionStatistics());
        // event (target)
        auto target = common::SessionStatisticsEvent{
            .message_info = message_info,
            .session_statistics = session_statistics};
        // validate
        compare(target.message_info, source.message_info);
        compare(target.session_statistics, source.session_statistics);
    }
}

TEST(flatbuffers, daily_statistics_event) {
    FlatBufferBuilder fbb;
    for (auto i = 0; i < MAX_ITERATIONS; ++i) {
        // reset
        fbb.Clear();  // doesn't de-allocate
        ASSERT_EQ(fbb.GetSize(), 0);
        // event (source)
        const auto source = common::DailyStatisticsEvent{
            .message_info = CreateRandomMessageInfo(),
            .daily_statistics = CreateRandomDailyStatistics()};
        // serialize using flatbuffers
        fbb.Finish(common::convert(fbb, source));
        ASSERT_GT(fbb.GetSize(), 0);
        // copy of the buffer (just making sure...)
        const std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
        // deserialize using flatbuffers
        auto root = GetRoot<schema::Event>(&buffer[0]);
        auto message_info = common::convert(root->message_info());
        ASSERT_EQ(root->event_data_type(), schema::EventData::DailyStatistics);
        auto daily_statistics = common::convert(root->event_data_as_DailyStatistics());
        // event (target)
        auto target = common::DailyStatisticsEvent{
            .message_info = message_info,
            .daily_statistics = daily_statistics};
        // validate
        compare(target.message_info, source.message_info);
        compare(target.daily_statistics, source.daily_statistics);
    }
}

TEST(flatbuffers, reference_data_event) {
    FlatBufferBuilder fbb;
    for (auto i = 0; i < MAX_ITERATIONS; ++i) {
        // reset
        fbb.Clear();  // doesn't de-allocate
        ASSERT_EQ(fbb.GetSize(), 0);
        // event (source)
        const auto source = common::ReferenceDataEvent{
            .message_info = CreateRandomMessageInfo(),
            .reference_data = CreateRandomReferenceData()};
        // serialize using flatbuffers
        fbb.Finish(common::convert(fbb, source));
        ASSERT_GT(fbb.GetSize(), 0);
        // copy of the buffer (just making sure...)
        const std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
        // deserialize using flatbuffers
        auto root = GetRoot<schema::Event>(&buffer[0]);
        auto message_info = common::convert(root->message_info());
        ASSERT_EQ(root->event_data_type(), schema::EventData::ReferenceData);
        auto reference_data = common::convert(root->event_data_as_ReferenceData());
        // event (target)
        auto target = common::ReferenceDataEvent{
            .message_info = message_info,
            .reference_data = reference_data};
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
        ASSERT_EQ(fbb.GetSize(), 0);
        // event (source)
        const auto source = common::MarketStatusEvent{
            .message_info = CreateRandomMessageInfo(),
            .market_status = CreateRandomMarketStatus()};
        // serialize using flatbuffers
        fbb.Finish(common::convert(fbb, source));
        ASSERT_GT(fbb.GetSize(), 0);
        // copy of the buffer (just making sure...)
        const std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
        // deserialize using flatbuffers
        auto root = GetRoot<schema::Event>(&buffer[0]);
        auto message_info = common::convert(root->message_info());
        ASSERT_EQ(root->event_data_type(), schema::EventData::MarketStatus);
        auto market_status = common::convert(root->event_data_as_MarketStatus());
        // event (target)
        auto target = common::MarketStatusEvent{
            .message_info = message_info,
            .market_status = market_status};
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
        ASSERT_EQ(fbb.GetSize(), 0);
        // event (source)
        const auto source = common::CreateOrderAckEvent{
            .message_info = CreateRandomMessageInfo(),
            .create_order_ack = CreateRandomCreateOrderAck()};
        // serialize using flatbuffers
        fbb.Finish(common::convert(fbb, source));
        ASSERT_GT(fbb.GetSize(), 0);
        // copy of the buffer (just making sure...)
        const std::vector<uint8_t> buffer(fbb.GetBufferPointer(), fbb.GetBufferPointer() + fbb.GetSize());
        // deserialize using flatbuffers
        auto root = GetRoot<schema::Event>(&buffer[0]);
        auto message_info = common::convert(root->message_info());
        ASSERT_EQ(root->event_data_type(), schema::EventData::CreateOrderAck);
        auto create_order_ack = common::convert(root->event_data_as_CreateOrderAck());
        // event (target)
        auto target = common::CreateOrderAckEvent{
            .message_info = message_info,
            .create_order_ack = create_order_ack};
        // validate
        compare(target.message_info, source.message_info);
        compare(target.create_order_ack, source.create_order_ack);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
