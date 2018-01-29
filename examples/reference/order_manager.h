/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <quinclas/tradingapi.h>
#include <unordered_map>
#include "reference/config.h"
#include "reference/risk_manager.h"

namespace examples {
namespace reference {

class OrderManager final {
 public:
  // constructor
  OrderManager(const Config& config, const RiskManager& risk_manager,
               quinclas::common::Strategy::Dispatcher& dispatcher);

 public:
  // event handlers
  void on(const quinclas::common::Timer& timer);
  void on(const quinclas::common::CreateOrderAck& create_order_ack);
  void on(const quinclas::common::ModifyOrderAck& modify_order_ack);
  void on(const quinclas::common::CancelOrderAck& cancel_order_ack);
  void on(const quinclas::common::OrderUpdate& order_update);

 public:
  // create order
  bool buy_ioc(double quantity, double limit_price);
  bool sell_ioc(double quantity, double limit_price);

 private:
  // internal create order
  bool create_order(const char *order_template,
                    quinclas::common::TradeDirection direction,
                    double quantity, double limit_price);

 private:
  // internal utilities
  double get_exposure(quinclas::common::TradeDirection direction) const;

 private:
  const Config& _config;
  const RiskManager& _risk_manager;
  quinclas::common::Strategy::Dispatcher& _dispatcher;

 private:
  // seed (opaque)
  // sent orders
  // cancelling orders
  // ack'ed orders
};

}  // namespace reference
}  // namespace examples

