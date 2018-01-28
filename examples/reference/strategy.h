/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <cctz/time_zone.h>

#include <string>
#include <utility>

#include <quinclas/tradingapi.h>

#include "reference/config.h"
#include "reference/order_manager.h"
#include "reference/position_manager.h"

namespace examples {
namespace reference {

class Strategy final : public quinclas::common::Strategy {
 public:
  // constructor
  Strategy(quinclas::common::Strategy::Dispatcher& dispatcher,
           const Config&& config);

 protected:
  // event handlers
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
  // state management
  void trade();

 private:
  // disallow default behaviour
  Strategy() = delete;
  Strategy(Strategy&) = delete;
  Strategy& operator=(Strategy&) = delete;

 private:
  quinclas::common::Strategy::Dispatcher& _dispatcher;
  const Config _config;
  PositionManager _position_manager;
  OrderManager _order_manager;
};

}  // namespace reference
}  // namespace examples
