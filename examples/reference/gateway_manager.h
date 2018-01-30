/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <cctz/time_zone.h>

#include <quinclas/tradingapi.h>

#include <string>
#include <utility>

#include "reference/config.h"
#include "reference/order_imbalance.h"
#include "reference/order_manager.h"
#include "reference/position_manager.h"
#include "reference/risk_manager.h"

namespace examples {
namespace reference {

class GatewayManager final : public quinclas::common::Strategy {
 public:
  // constructor
  GatewayManager(quinclas::common::Strategy::Dispatcher& dispatcher,
                 Config&& config);

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
  GatewayManager() = delete;
  GatewayManager(GatewayManager&) = delete;
  GatewayManager& operator=(GatewayManager&) = delete;

 private:
  quinclas::common::Strategy::Dispatcher& _dispatcher;
  Config _config;
  PositionManager _position_manager;
  RiskManager _risk_manager;
  OrderManager _order_manager;
  OrderImbalance _order_imbalance;
  bool _order_manager_ready = false;
  bool _market_data_ready = false;
  bool _market_open = false;
};

}  // namespace reference
}  // namespace examples
