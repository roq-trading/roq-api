/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

namespace examples {
namespace strategy {

// Example of a strategy implementation.
// The implementation makes no sense -- it is only meant to show the "how-to"s.
class Strategy final : public roq::common::Strategy {
 public:
  // The first argument to the constructor must always be an interface to a
  // request dispatcher.
  // Any further arguments are optional -- here we have a simple configuration
  // with number of ticks required before we send a single trade.
  Strategy(roq::common::Strategy::Dispatcher& dispatcher,
           uint32_t ticks_to_trade);

 protected:
  // You must implement event handlers;
  // Internal communication between controller and strategy
  void on(const roq::common::TimerEvent&) override;
  void on(const roq::common::ConnectionStatusEvent&) override;
  // Identify batch boundaries (when receiving correlated events)
  void on(const roq::common::BatchBeginEvent&) override;
  void on(const roq::common::BatchEndEvent&) override;
  // This event will be received when the client can send requests
  void on(const roq::common::ReadyEvent&) override;
  // The gateway's internal status
  void on(const roq::common::GatewayStatusEvent&) override;
  // Reference data events
  void on(const roq::common::ReferenceDataEvent&) override;
  void on(const roq::common::MarketStatusEvent&) override;
  // Market data events
  void on(const roq::common::MarketByPriceEvent&) override;
  void on(const roq::common::TradeSummaryEvent&) override;
  // Order management events
  void on(const roq::common::CreateOrderAckEvent&) override;
  void on(const roq::common::ModifyOrderAckEvent&) override;
  void on(const roq::common::CancelOrderAckEvent&) override;
  void on(const roq::common::OrderUpdateEvent&) override;
  void on(const roq::common::TradeUpdateEvent&) override;
  void on(const roq::common::PositionUpdateEvent&) override;

 private:
  roq::common::Strategy::Dispatcher& _dispatcher;
  const uint32_t _ticks_to_trade;
  bool _market_data_ready = false;
  bool _order_management_ready = false;
};

}  // namespace strategy
}  // namespace examples
