/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/order_manager.h"
#include <glog/logging.h>
#include <limits>

namespace examples {
namespace reference {

// constructor

OrderManager::OrderManager(const Config& config, const RiskManager& risk_manager,
                           quinclas::common::Strategy::Dispatcher& dispatcher)
    : _config(config), _risk_manager(risk_manager), _dispatcher(dispatcher) {}

// event handlers

void OrderManager::on(const quinclas::common::Timer& timer) {
}

void OrderManager::on(const quinclas::common::CreateOrderAck& create_order_ack) {
}

void OrderManager::on(const quinclas::common::ModifyOrderAck& modify_order_ack) {
}

void OrderManager::on(const quinclas::common::CancelOrderAck& cancel_order_ack) {
}

void OrderManager::on(const quinclas::common::OrderUpdate& order_update) {
}

// create order

void OrderManager::buy_ioc(const double quantity, const double limit_price) {
  create_order("ioc", quinclas::common::TradeDirection::Buy, quantity, limit_price);
}

void OrderManager::sell_ioc(const double quantity, const double limit_price) {
  create_order("ioc", quinclas::common::TradeDirection::Sell, quantity, limit_price);
}

void OrderManager::create_order(const char *order_template,
                                const quinclas::common::TradeDirection direction,
                                const double quantity, const double limit_price) {
  quinclas::common::CreateOrder create_order {
    .opaque         = 0,
    .order_template = order_template,
    .exchange       = _config.exchange.c_str(),
    .instrument     = _config.instrument.c_str(),
    .direction      = direction,
    .quantity       = quantity,
    .limit_price    = limit_price,
    .stop_price     = std::numeric_limits<double>::quiet_NaN(),
  };
  _dispatcher.send("FEMAS", create_order);
}

}  // namespace reference
}  // namespace examples
