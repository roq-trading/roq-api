/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/order_manager.h"
#include <glog/logging.h>
#include <limits>

namespace examples {
namespace reference {

const char *GATEWAY = "FEMAS";
const char *IOC = "ioc";

namespace {
// TODO(thraneh): this is a utility funtion!
static double get_real_quantity(const quinclas::common::TradeDirection direction, const double quantity) {
  switch (direction) {
    case quinclas::common::TradeDirection::Buy:
      return quantity;
    case quinclas::common::TradeDirection::Sell:
      return -quantity;
    default:
      LOG(FATAL) << "Received unknown trade direction!";
  }
}
}  // namespace

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

bool OrderManager::buy_ioc(const double quantity, const double limit_price) {
  return create_order(IOC, quinclas::common::TradeDirection::Buy, quantity, limit_price);
}

bool OrderManager::sell_ioc(const double quantity, const double limit_price) {
  return create_order(IOC, quinclas::common::TradeDirection::Sell, quantity, limit_price);
}

bool OrderManager::create_order(const char *order_template,
                                const quinclas::common::TradeDirection direction,
                                const double quantity, const double limit_price) {
  auto real_quantity = get_real_quantity(direction, quantity);
  auto available_quantity = _risk_manager.available_quantity(_config.instrument, direction);
  if (true) {  // TODO(thraneh): check if we have headroom
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
    return true;
  } else {
    LOG(WARNING) << "Unable to sell instrument=" << _config.instrument << "!";
    return false;
  }
}

}  // namespace reference
}  // namespace examples
