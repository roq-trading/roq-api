/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/order_manager.h"
#include <glog/logging.h>
#include <limits>

namespace examples {
namespace reference {

const char *GATEWAY = "FEMAS";
const char *IOC = "ioc";

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

bool OrderManager::buy_ioc(double quantity, double limit_price) {
  return create_order(IOC, quinclas::common::TradeDirection::Buy,
                      quantity, limit_price);
}

bool OrderManager::sell_ioc(double quantity, double limit_price) {
  return create_order(IOC, quinclas::common::TradeDirection::Sell,
                      quantity, limit_price);
}

bool OrderManager::create_order(const char *order_template,
                                quinclas::common::TradeDirection direction,
                                double quantity, double limit_price) {
  // risk manager must confirm
  double exposure = get_exposure(direction);
  if (!_risk_manager.can_trade(_config.instrument, direction,
                               quantity, exposure))
    return false;
  // send the new order
  try {
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
    _dispatcher.send(GATEWAY, create_order);
  } catch (std::exception& e) {
    LOG(WARNING) << e.what();
    return false;
  }
  return true;
}

double OrderManager::get_exposure(quinclas::common::TradeDirection direction) const {
  return 0.0;  // TODO(thraneh): implement
}

}  // namespace reference
}  // namespace examples
