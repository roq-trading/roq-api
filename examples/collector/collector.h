/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <chrono>  // NOLINT
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace examples {
namespace collector {

class Collector final : public roq::common::Strategy {
 public:
  explicit Collector(roq::common::Strategy::Dispatcher& dispatcher) : _dispatcher(dispatcher) {}

  struct State final {
    explicit State(const std::string& instrument) : instrument(instrument) {}
    void update(const roq::common::MarketByPriceEvent&);
    void update(const roq::common::TradeSummaryEvent&);
    std::string instrument;
    roq::common::time_point_t exchange_time;
    roq::common::time_point_t receive_time;
    roq::common::Layer depth[5] = {};
    double price = 0.0;
    double volume = 0.0;
    double turnover = 0.0;
  };

 protected:
  void on(const roq::common::TimerEvent&) override {}
  void on(const roq::common::ConnectionStatusEvent&) override {}
  void on(const roq::common::BatchBeginEvent&) override {}
  void on(const roq::common::BatchEndEvent&) override;
  void on(const roq::common::ReadyEvent&) override {}
  void on(const roq::common::GatewayStatusEvent&) override {}
  void on(const roq::common::ReferenceDataEvent&) override {}
  void on(const roq::common::MarketStatusEvent&) override {}
  void on(const roq::common::MarketByPriceEvent&) override;
  void on(const roq::common::TradeSummaryEvent&) override;
  void on(const roq::common::CreateOrderAckEvent&) override {}
  void on(const roq::common::ModifyOrderAckEvent&) override {}
  void on(const roq::common::CancelOrderAckEvent&) override {}
  void on(const roq::common::OrderUpdateEvent&) override {}
  void on(const roq::common::TradeUpdateEvent&) override {}
  void on(const roq::common::PositionUpdateEvent&) override {}

  State& get(const std::string& instrument);

 private:
  roq::common::Strategy::Dispatcher& _dispatcher;
  std::unordered_map<std::string, State> _cache;
  std::unordered_set<State*> _dirty;
};

std::ostream& operator<<(std::ostream&, Collector::State&);

}  // namespace collector
}  // namespace examples
