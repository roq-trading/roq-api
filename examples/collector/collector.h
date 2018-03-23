/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <quinclas/tradingapi.h>

#include <chrono>  // NOLINT
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace examples {
namespace collector {

class Collector final : public quinclas::common::Strategy {
 public:
  explicit Collector(quinclas::common::Strategy::Dispatcher& dispatcher) : _dispatcher(dispatcher) {}

  struct State final {
    explicit State(const std::string& instrument) : instrument(instrument) {}
    void update(const quinclas::common::MarketByPriceEvent&);
    void update(const quinclas::common::TradeSummaryEvent&);
    std::string instrument;
    quinclas::common::time_point_t exchange_time;
    quinclas::common::time_point_t receive_time;
    quinclas::common::Layer depth[5] = {};
    double price = 0.0;
    double volume = 0.0;
    double turnover = 0.0;
  };

 protected:
  void on(const quinclas::common::TimerEvent&) override {}
  void on(const quinclas::common::ConnectionStatusEvent&) override {}
  void on(const quinclas::common::BatchBeginEvent&) override {}
  void on(const quinclas::common::BatchEndEvent&) override;
  void on(const quinclas::common::ReadyEvent&) override {}
  void on(const quinclas::common::GatewayStatusEvent&) override {}
  void on(const quinclas::common::ReferenceDataEvent&) override {}
  void on(const quinclas::common::MarketStatusEvent&) override {}
  void on(const quinclas::common::MarketByPriceEvent&) override;
  void on(const quinclas::common::TradeSummaryEvent&) override;
  void on(const quinclas::common::CreateOrderAckEvent&) override {}
  void on(const quinclas::common::ModifyOrderAckEvent&) override {}
  void on(const quinclas::common::CancelOrderAckEvent&) override {}
  void on(const quinclas::common::OrderUpdateEvent&) override {}
  void on(const quinclas::common::TradeUpdateEvent&) override {}
  void on(const quinclas::common::PositionUpdateEvent&) override {}

  State& get(const std::string& instrument);

 private:
  quinclas::common::Strategy::Dispatcher& _dispatcher;
  std::unordered_map<std::string, State> _cache;
  std::unordered_set<State*> _dirty;
};

std::ostream& operator<<(std::ostream&, Collector::State&);

}  // namespace collector
}  // namespace examples
