/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <cctz/time_zone.h>

#include <quinclas/tradingapi.h>

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

class GatewayManager final : public quinclas::common::Strategy {
 public:
  // constructor
  GatewayManager(quinclas::common::Strategy::Dispatcher& dispatcher,
                 Config&& config);

 protected:
  // event handlers
  void on(const quinclas::common::TimerEvent&) override;
  void on(const quinclas::common::ConnectionStatusEvent&) override;
  void on(const quinclas::common::BatchBeginEvent&) override;
  void on(const quinclas::common::BatchEndEvent&) override;
  void on(const quinclas::common::ReadyEvent&) override;
  void on(const quinclas::common::GatewayStatusEvent&) override;
  void on(const quinclas::common::ReferenceDataEvent&) override;
  void on(const quinclas::common::MarketStatusEvent&) override;
  void on(const quinclas::common::MarketByPriceEvent&) override;
  void on(const quinclas::common::TradeSummaryEvent&) override;
  void on(const quinclas::common::CreateOrderAckEvent&) override;
  void on(const quinclas::common::ModifyOrderAckEvent&) override;
  void on(const quinclas::common::CancelOrderAckEvent&) override;
  void on(const quinclas::common::OrderUpdateEvent&) override;
  void on(const quinclas::common::TradeUpdateEvent&) override;
  void on(const quinclas::common::PositionUpdateEvent&) override;

 private:
  // state management
  void check(const quinclas::common::MessageInfo&);

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
