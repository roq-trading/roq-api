/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/order_manager.h"
#include <glog/logging.h>
#include <algorithm>
#include <limits>
#include "reference/utilities.h"

namespace examples {
namespace reference {

const char *GATEWAY = "FEMAS";

// constructor

OrderManager::OrderManager(const Config& config, const RiskManager& risk_manager,
                           quinclas::common::Strategy::Dispatcher& dispatcher)
    : _config(config), _risk_manager(risk_manager), _dispatcher(dispatcher) {}


// create order

uint32_t OrderManager::buy(const char *order_template, double quantity,
                           double limit_price) {
  return create_order(order_template, quinclas::common::TradeDirection::Buy,
                      quantity, limit_price);
}

uint32_t OrderManager::sell(const char *order_template, double quantity,
                            double limit_price) {
  return create_order(order_template, quinclas::common::TradeDirection::Sell,
                      quantity, limit_price);
}

uint32_t OrderManager::create_order(const char *order_template,
                                    quinclas::common::TradeDirection direction,
                                    double quantity, double limit_price) {
  // risk manager
  // TODO(thraneh): here we need to access a global "time" !!!
  double exposure = get_exposure(direction);
  if (!_risk_manager.can_trade(_config.instrument, direction,
                               quantity, exposure))
    return 0;
  // create order
  auto order_id = ++_next_order_id;
  try {
    quinclas::common::CreateOrder create_order {
      .opaque         = order_id,
      .order_template = order_template,
      .exchange       = _config.exchange.c_str(),
      .instrument     = _config.instrument.c_str(),
      .direction      = direction,
      .quantity       = quantity,
      .limit_price    = limit_price,
      .stop_price     = std::numeric_limits<double>::quiet_NaN(),
    };
    _dispatcher.send(GATEWAY, create_order);
  } catch (std::exception& e) {  // TODO(thraneh): more specific type(s)
    LOG(WARNING) << e.what();
    return 0;
  }
  _orders.emplace(order_id, Order(direction, quantity));
  _timeout.emplace_back(_last_update_time + _config.order_timeout, order_id);
  update_exposure(direction, quantity);
  return order_id;
}

void OrderManager::update_order(uint32_t order_id, uint32_t gateway_order_id,
                                double remaining_quantity) {
  auto iter = _orders.find(order_id);
  if (iter == _orders.end())
    return;
  auto& order = (*iter).second;
  order.gateway_order_id = gateway_order_id;
  auto delta_quantity = remaining_quantity - order.remaining_quantity;
  if (is_equal(delta_quantity, 0.0))
    return;
  order.remaining_quantity = remaining_quantity;
  update_exposure(order.direction, delta_quantity);
}

void OrderManager::remove_order(uint32_t order_id) {
  auto iter = _orders.find(order_id);
  if (iter == _orders.end())
    return;
  auto& order = (*iter).second;
  update_exposure(order.direction, -order.remaining_quantity);
  _orders.erase(iter);
}

double OrderManager::get_exposure(
    quinclas::common::TradeDirection direction) const {
  switch (direction) {
    case quinclas::common::TradeDirection::Undefined:
      LOG(FATAL) << "Unexpected direction";
    case quinclas::common::TradeDirection::Buy:
      return _buy_exposure;
    case quinclas::common::TradeDirection::Sell:
      return _sell_exposure;
  }
}

void OrderManager::update_exposure(quinclas::common::TradeDirection direction,
                                   double delta_quantity) {
  switch (direction) {
    case quinclas::common::TradeDirection::Undefined:
      LOG(FATAL) << "Unexpected direction";
    case quinclas::common::TradeDirection::Buy:
      _buy_exposure = std::max(_buy_exposure + delta_quantity, 0.0);
      break;
    case quinclas::common::TradeDirection::Sell:
      _sell_exposure = std::max(_sell_exposure + delta_quantity, 0.0);
      break;
  }
}

// slow -- use only for testing and debug
void OrderManager::compute_and_update_exposure() {
  double buy_exposure = 0.0, sell_exposure = 0.0;
  for (const auto iter : _orders) {
    const auto& order = iter.second;
    switch (order.direction) {
      case quinclas::common::TradeDirection::Undefined:
        LOG(FATAL) << "Unexpected direction";
      case quinclas::common::TradeDirection::Buy:
        buy_exposure += order.remaining_quantity;
        break;
      case quinclas::common::TradeDirection::Sell:
        sell_exposure += order.remaining_quantity;
        break;
    }
  }
  _buy_exposure = buy_exposure;
  _sell_exposure = sell_exposure;
}

// event handlers

void OrderManager::on(const quinclas::common::TimerEvent& event) {
  check(event.message_info);
}

void OrderManager::on(const quinclas::common::CreateOrderAckEvent& event) {
  check(event.message_info);
  const auto& create_order_ack = event.create_order_ack;
  auto order_id = create_order_ack.opaque;
  if (create_order_ack.failure) {
    remove_order(order_id);
  } else {
    auto iter = _orders.find(order_id);
    if (iter == _orders.end())
      return;
    auto& order = (*iter).second;
    order.gateway_order_id = create_order_ack.order_id;
  }
}

void OrderManager::on(const quinclas::common::ModifyOrderAckEvent& event) {
  check(event.message_info);
  const auto& modify_order_ack = event.modify_order_ack;
  auto order_id = modify_order_ack.opaque;
  if (modify_order_ack.failure) {
    LOG(WARNING) << "Failed to modify the order!";
  } else {
    auto iter = _orders.find(order_id);
    if (iter == _orders.end())
      return;
    auto& order = (*iter).second;
    // TODO(thraneh): this feels unsafe
    update_exposure(order.direction, modify_order_ack.quantity_change);
  }
}

void OrderManager::on(const quinclas::common::CancelOrderAckEvent& event) {
  check(event.message_info);
  const auto& cancel_order_ack = event.cancel_order_ack;
  auto order_id = cancel_order_ack.opaque;
  if (cancel_order_ack.failure)
    LOG(WARNING) << "Failed to cancel the order!";
  else
    remove_order(order_id);
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
  if (remove) {
    remove_order(order_id);
  } else {
    if (timer)
      _timeout.emplace_back(_last_update_time + _config.order_timeout, order_id);
    // TODO(thraneh): this feels unsafe
    auto delta_quantity = order_update.remaining_quantity
                        - order.remaining_quantity;
    update_exposure(order.direction, delta_quantity);
  }
}

void OrderManager::check(const quinclas::common::MessageInfo& message_info) {
  auto current_time = message_info.enqueue_time;
  LOG_IF(FATAL, _last_update_time < current_time) << "Wrong sequencing!";
  _last_update_time = current_time;
  // check timeout
  while (!_timeout.empty()) {
    const auto& front = _timeout.front();
    if (current_time < front.first)
      break;
    remove_order(front.second);
    _timeout.pop_front();
  }
}

}  // namespace reference
}  // namespace examples
