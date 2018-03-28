/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <cctz/time_zone.h>

#include <roq/api.h>

#include <chrono>  // NOLINT
#include <string>
#include <utility>

#include "reference/config.h"
#include "reference/order_manager.h"
#include "reference/position_manager.h"
#include "reference/risk_manager.h"
#include "reference/trading_model.h"

namespace examples {
namespace reference {

class GatewayManager final : public roq::common::Strategy {
 public:
  // constructor
  GatewayManager(roq::common::Strategy::Dispatcher& dispatcher,
                 Config&& config);

 protected:
  // event handlers
  void on(const roq::common::TimerEvent&) override;
  void on(const roq::common::ConnectionStatusEvent&) override;
  void on(const roq::common::BatchBeginEvent&) override;
  void on(const roq::common::BatchEndEvent&) override;
  void on(const roq::common::ReadyEvent&) override;
  void on(const roq::common::GatewayStatusEvent&) override;
  void on(const roq::common::ReferenceDataEvent&) override;
  void on(const roq::common::MarketStatusEvent&) override;
  void on(const roq::common::MarketByPriceEvent&) override;
  void on(const roq::common::TradeSummaryEvent&) override;
  void on(const roq::common::CreateOrderAckEvent&) override;
  void on(const roq::common::ModifyOrderAckEvent&) override;
  void on(const roq::common::CancelOrderAckEvent&) override;
  void on(const roq::common::OrderUpdateEvent&) override;
  void on(const roq::common::TradeUpdateEvent&) override;
  void on(const roq::common::PositionUpdateEvent&) override;

 private:
  // state management
  void check(const roq::common::MessageInfo&);

 private:
  // disallow default behaviour
  GatewayManager() = delete;
  GatewayManager(GatewayManager&) = delete;
  GatewayManager& operator=(GatewayManager&) = delete;

 private:
  roq::common::Strategy::Dispatcher& _dispatcher;
  Config _config;
  PositionManager _position_manager;
  RiskManager _risk_manager;
  OrderManager _order_manager;
  TradingModel _trading_model;
  // state management
  bool _order_manager_ready = false;
  bool _market_data_ready = false;
  bool _market_open = false;
  // consistency check
  typedef std::chrono::system_clock::time_point time_point_t;
  time_point_t _last_update_time;
};

}  // namespace reference
}  // namespace examples
