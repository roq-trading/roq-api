/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <benchmark/benchmark.h>

#include <roq/api.h>
#include <roq/codec.h>

// CreateOrder

class CreateOrderFixture : public ::benchmark::Fixture {
 public:
  void SetUp(const ::benchmark::State&) {}
  void TearDown(const ::benchmark::State&) {}

 protected:
  flatbuffers::FlatBufferBuilder _fbb;
};
BENCHMARK_DEFINE_F(CreateOrderFixture, CreateOrderTest)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto create_order = roq::CreateOrder {
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
    _fbb.Clear();
    _fbb.Finish(roq::codec::convert(_fbb, create_order));
  }
}
BENCHMARK_REGISTER_F(CreateOrderFixture, CreateOrderTest);

// CreateOrderEvent

class CreateOrderEventFixture : public ::benchmark::Fixture {
 public:
  void SetUp(const ::benchmark::State&) {}
  void TearDown(const ::benchmark::State&) {}

 protected:
  flatbuffers::FlatBufferBuilder _fbb;
  std::chrono::system_clock::time_point _now = std::chrono::system_clock::now();
};
BENCHMARK_DEFINE_F(CreateOrderEventFixture, CreateOrderEventTest)(
    benchmark::State& state) {
  for (auto _ : state) {
    auto source_info = roq::SourceInfo {
      .seqno = 12345678,
      .create_time = _now,
    };
    auto create_order = roq::CreateOrder {
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
    _fbb.Clear();
    _fbb.Finish(roq::codec::convert2(_fbb, source_info, create_order));
  }
}
BENCHMARK_REGISTER_F(CreateOrderEventFixture, CreateOrderEventTest);

BENCHMARK_MAIN();
