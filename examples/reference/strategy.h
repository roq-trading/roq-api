/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <cctz/time_zone.h>

#include <string>
#include <utility>

#include <quinclas/tradingapi.h>

namespace examples {
namespace reference {

class Strategy final : public quinclas::common::Strategy {
 public:
  struct Config {
    cctz::time_zone time_zone;
    std::string instrument;
    std::string exchange;
    std::pair<double, double> model_params;
  };

 public:
  Strategy(quinclas::common::Strategy::Dispatcher& dispatcher, Config&& config);

 protected:
  void on(const quinclas::common::TimerEvent&) override;
  void on(const quinclas::common::IdleEvent&) override;
  void on(const quinclas::common::GatewayStatusEvent&) override;
  void on(const quinclas::common::ReferenceDataEvent&) override;
  void on(const quinclas::common::MarketStatusEvent&) override;
  void on(const quinclas::common::MarketByPriceEvent&) override;
  void on(const quinclas::common::SessionStatisticsEvent&) override;
  void on(const quinclas::common::DailyStatisticsEvent&) override;
  void on(const quinclas::common::CreateOrderAckEvent&) override;
  void on(const quinclas::common::ModifyOrderAckEvent&) override;
  void on(const quinclas::common::CancelOrderAckEvent&) override;
  void on(const quinclas::common::OrderUpdateEvent&) override;
  void on(const quinclas::common::TradeUpdateEvent&) override;

 private:
  Strategy() = delete;
  Strategy(Strategy&) = delete;
  Strategy& operator=(Strategy&) = delete;

 private:
  quinclas::common::Strategy::Dispatcher& _dispatcher;
  Config _config;
};

}  // namespace reference
}  // namespace examples
