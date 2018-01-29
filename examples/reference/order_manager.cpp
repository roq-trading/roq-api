/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/order_manager.h"
#include <glog/logging.h>
#include <limits>
#include "reference/utilities.h"

namespace examples {
namespace reference {

const char *GATEWAY = "FEMAS";
const char *IOC = "ioc";

// constructor

OrderManager::OrderManager(const Config& config, const RiskManager& risk_manager,
                           quinclas::common::Strategy::Dispatcher& dispatcher)
    : _config(config), _risk_manager(risk_manager), _dispatcher(dispatcher) {}

// event handlers

void OrderManager::on(const quinclas::common::TimerEvent& event) {
  check(event.message_info);
}

void OrderManager::on(const quinclas::common::CreateOrderAckEvent& event) {
  check(event.message_info);
  const auto& create_order_ack = event.create_order_ack;
  auto order_id = create_order_ack.opaque;
  auto iter = _orders.find(order_id);
  if (iter == _orders.end())
    return;
  if (create_order_ack.failure) {
    _orders.erase(iter);
  } else {
    auto& order = (*iter).second;
    order.gateway_order_id = create_order_ack.order_id;
  }
}

void OrderManager::on(const quinclas::common::ModifyOrderAckEvent& event) {
  check(event.message_info);
  // TODO(thraneh): implement
}

void OrderManager::on(const quinclas::common::CancelOrderAckEvent& event) {
  check(event.message_info);
  // TODO(thraneh): implement
}

void OrderManager::on(const quinclas::common::OrderUpdateEvent& event) {
  check(event.message_info);
  const auto& order_update = event.order_update;
  auto order_id = order_update.opaque;
  auto iter = _orders.find(order_id);
  if (iter == _orders.end()) {
    LOG(WARNING) << "Got update for non-existing order!";
    return;
  }
  auto& order = (*iter).second;
  bool remove = false, timer = false;
  switch (order_update.status) {
    case quinclas::common::OrderStatus::Undefined:
      LOG(FATAL) << "Unexpected order status!";
      break;
    case quinclas::common::OrderStatus::Sent:
      if (order.state == Order::Requested) {
          timer = true;
          order.state = Order::Sent;
      }
      break;
    case quinclas::common::OrderStatus::Failed:
      remove = true;
      break;
    case quinclas::common::OrderStatus::Accepted:
      order.state = Order::Working;
      LOG_IF(FATAL, is_equal(order.remaining_quantity, 0.0)) << "Unexpected!";
      break;
    case quinclas::common::OrderStatus::Cancelled:
      remove = true;
      break;
    case quinclas::common::OrderStatus::Filled:
      order.state = Order::Working;
      remove = is_equal(order.remaining_quantity, 0.0);
      break;
  }
  if (remove)
    _orders.erase(iter);
  else if (timer)
    _timeout.push_back(std::make_pair(_last_update_time + _config.order_timeout, order_id));
  // TODO(thraneh): update exposure
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
  // TODO(thraneh): we need a global "time" !!!
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
  // TODO(thraneh): add to exposure
  auto order_id = ++_next_internal_id;
  _orders.emplace(order_id, Order(direction, quantity));
  _timeout.push_back(std::make_pair(_last_update_time + _config.order_timeout, order_id));
  return true;
}

double OrderManager::get_exposure(quinclas::common::TradeDirection direction) const {
  return 0.0;  // TODO(thraneh): implement
}

void OrderManager::check(const quinclas::common::MessageInfo& message_info) {
  auto current_time = message_info.enqueue_time;
  LOG_IF(FATAL, _last_update_time < current_time) << "Detected wrong sequencing!";
  _last_update_time = current_time;
  // check timeout
  while (!_timeout.empty()) {
    const auto& front = _timeout.front();
    if (current_time < front.first)
      break;
    auto order_id = front.second;
    _timeout.pop_front();
    auto iter = _orders.find(order_id);
    if (iter == _orders.end())
      return;
    LOG(WARNING) << "Order (id: " << order_id << ") has timed out!";
    _orders.erase(iter);
  }
}

}  // namespace reference
}  // namespace examples
