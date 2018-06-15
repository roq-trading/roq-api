/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <benchmark/benchmark.h>

#include <roq/api.h>
#include <roq/codec.h>
#include <roq/stream.h>

namespace {
inline roq::SourceInfo create_source_info(
    std::chrono::system_clock::time_point now) {
  return roq::SourceInfo {
    .seqno = 12345678,
    .create_time = now,
  };
}
inline roq::CreateOrder create_create_order() {
  return roq::CreateOrder {
    .account = "01234567",
    .order_id = 1234,
    .exchange = "CFFEX",
    .symbol = "IC1806",
    .side = roq::Side::Buy,
    .quantity = 1234.5,
    .order_type = roq::OrderType::Market,
    .limit_price = 123.45,
    .time_in_force = roq::TimeInForce::IOC,
    .position_effect = roq::PositionEffect::Close,
    .order_template = "default",
  };
}
}  // namespace

// EncodeFixture
class EncodeFixture : public ::benchmark::Fixture {
 protected:
  // typical pattern is to re-use the same flatbuffers builder
  flatbuffers::FlatBufferBuilder _fbb;
  // externalize the access to system resources
  std::chrono::system_clock::time_point _now = std::chrono::system_clock::now();
};

// BM_CreateOrder_Encode
BENCHMARK_DEFINE_F(EncodeFixture, BM_CreateOrder_Encode)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto create_order = create_create_order();
    _fbb.Clear();
    _fbb.Finish(
        roq::codec::convert(
            _fbb,
            create_order));
  }
}
BENCHMARK_REGISTER_F(EncodeFixture, BM_CreateOrder_Encode);

// BM_CreateOrderEvent_Encode

BENCHMARK_DEFINE_F(EncodeFixture, BM_CreateOrderEvent_Encode)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto source_info = create_source_info(_now);
    auto create_order = create_create_order();
    _fbb.Clear();
    _fbb.Finish(
        roq::codec::convert2(
            _fbb,
            source_info,
            create_order));
  }
}
BENCHMARK_REGISTER_F(EncodeFixture, BM_CreateOrderEvent_Encode);

// BM_CreateOrder_Stream
void BM_CreateOrder_Stream(
    benchmark::State& state) {
  for (auto _ : state) {
    auto create_order = create_create_order();
    std::stringstream ss;
    ss << create_order;
    auto str = ss.str();
  }
}
BENCHMARK(BM_CreateOrder_Stream);

BENCHMARK_MAIN();
