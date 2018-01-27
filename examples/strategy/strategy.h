/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <quinclas/tradingapi.h>

namespace examples {
namespace strategy {

// Example of a strategy implementation.
// The implementation makes no sense -- it is only meant to show the "how-to"s.
class Strategy final : public quinclas::common::Strategy {
 public:
  // The first argument to the constructor must always be an interface to a
  // request dispatcher.
  // Any further arguments are optional -- here we have a simple configuration
  // with number of ticks required before we send a single trade.
  Strategy(quinclas::common::Strategy::Dispatcher& dispatcher,
           uint32_t ticks_to_trade);

 protected:
  // You must implement event handlers;
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
  quinclas::common::Strategy::Dispatcher& _dispatcher;
  const uint32_t _ticks_to_trade;
};

}  // namespace strategy
}  // namespace examples
