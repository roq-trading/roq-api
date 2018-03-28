/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/order_manager.h"
#include <roq/logging.h>
#include <algorithm>
#include <limits>
#include "reference/utilities.h"

namespace examples {
namespace reference {

const char *GATEWAY = "FEMAS";

// constructor

OrderManager::OrderManager(const Config& config, const RiskManager& risk_manager,
                           roq::common::Strategy::Dispatcher& dispatcher)
    : _config(config), _risk_manager(risk_manager), _dispatcher(dispatcher) {}


// create order request

uint32_t OrderManager::buy(const char *order_template, double quantity,
                           double limit_price) {
  return create_order(order_template, roq::common::TradeDirection::Buy,
                      quantity, limit_price);
}

uint32_t OrderManager::sell(const char *order_template, double quantity,
                            double limit_price) {
  return create_order(order_template, roq::common::TradeDirection::Sell,
                      quantity, limit_price);
}

uint32_t OrderManager::create_order(const char *order_template,
                                    roq::common::TradeDirection direction,
                                    double quantity, double limit_price) {
  // manage exposure by direction -- we can't trade if we will breach a limit
  auto exposure = _exposure.get(direction);
  if (!_risk_manager.can_trade(_config.instrument, direction,
                               quantity, exposure)) {
    return 0;
  }
  // risk manager was happy, now try to send the order
  auto order_id = ++_next_order_id;
  try {
    roq::common::CreateOrder create_order {
      .opaque         = order_id,
      .order_template = order_template,
      .exchange       = _config.exchange.c_str(),
      .instrument     = _config.instrument.c_str(),
      .direction      = direction,
      .quantity       = quantity,
      .limit_price    = limit_price,
      .stop_price     = std::numeric_limits<double>::quiet_NaN(),
    };
    _dispatcher.send(create_order, GATEWAY);
  } catch (std::exception& e) {  // TODO(thraneh): more specific type(s)
    // could also let the exception reach the user
    LOG(WARNING) << e.what();
    return 0;
  }
  // state management for this order
  _orders.emplace(order_id, Order(direction, quantity));
  // request (future) check for timeout
  add_timeout_check(order_id);
  // update exposure
  _exposure.update(Exposure::Create, direction, quantity);
  // finally return the id of the new order -- the user should use this id to modify and/or cancel the order
  return order_id;
}

// event handlers

void OrderManager::on(const roq::common::TimerEvent& event) {
  // check(event.message_info);  // FIXME(thraneh): re-enable time-check
}

void OrderManager::on(const roq::common::CreateOrderAckEvent& event) {
  check(event.message_info);
  const auto& create_order_ack = event.create_order_ack;
  auto order_id = create_order_ack.opaque;
  auto iter = _orders.find(order_id);
  if (iter == _orders.end())
    return;
  auto& order = (*iter).second;
  if (create_order_ack.failure) {
    LOG_IF(FATAL, Order::Sent < order.state) << "Unexpected -- order was previously accepted or working";
    // update exposure
    _exposure.update(Exposure::Reject, order.direction, order.remaining_quantity);
    // remove the order -- we don't expect any further updates
    _orders.erase(iter);
  } else {
    // record the order id known to the gateway -- we need it if the user later want to modify or cancel
    if (create_order_ack.order_id > 0)
      order.gateway_order_id = create_order_ack.order_id;
  }
}

void OrderManager::on(const roq::common::ModifyOrderAckEvent& event) {
  check(event.message_info);
  // significantly more complex exposure management -- not implemented for this example
  LOG(FATAL) << "Order modifications not supported!";
}

void OrderManager::on(const roq::common::CancelOrderAckEvent& event) {
  check(event.message_info);
  const auto& cancel_order_ack = event.cancel_order_ack;
  auto order_id = cancel_order_ack.opaque;
  if (cancel_order_ack.failure) {
    LOG(WARNING) << "Order cancellation did not succeed!";
  } else {
    auto iter = _orders.find(order_id);
    if (iter == _orders.end())
      return;
    // expect this event to be followed by an order update (there could be fills)
    auto& order = (*iter).second;
    order.state = Order::Deleting;
    // add a timeout (in case we don't get the order update)
    add_timeout_check(order_id);
  }
}

void OrderManager::on(const roq::common::OrderUpdateEvent& event) {
  check(event.message_info);
  const auto& order_update = event.order_update;
  auto order_id = order_update.opaque;
  auto iter = _orders.find(order_id);
  if (iter == _orders.end()) {
    LOG(WARNING) << "Got update for non-existing order! (The order could already have timed out).";
    return;
  }
  auto& order = (*iter).second;
  // get current fill -- order will be updated with remaining quantity
  auto fill = order.fill(order_update);
  bool has_fill = !is_equal(fill, 0.0);
  if (has_fill)
    _exposure.update(Exposure::Fill, order.direction, fill);
  switch (order_update.status) {
    case roq::common::OrderStatus::Undefined: {
      // this is wrong -- an (unknown? new?) enum has probably not been processed by the gateway
      LOG(FATAL) << "Unexpected order status!";
      break;
    }
    case roq::common::OrderStatus::Sent: {
      // the order has been confirmed sent by the gateway -- let's check back later (for timeout)
      if (order.state < Order::Sent) {
          order.state = Order::Sent;
          add_timeout_check(order_id);
      }
      break;
    }
    case roq::common::OrderStatus::Rejected: {
      // the order was rejected (could be any number of reasons)
      LOG_IF(FATAL, has_fill) << "Unexpected!";
      _exposure.update(Exposure::Reject, order.direction, order.remaining_quantity);
      _orders.erase(order_id);
      break;
    }
    case roq::common::OrderStatus::Accepted: {
      // the order has been received by broker or exchange
      if (order.state < Order::Accepted)
        order.state = Order::Accepted;
      LOG_IF(FATAL, has_fill) << "Unexpected!";
      break;
    }
    case roq::common::OrderStatus::Pending: {
      // the order has been registered on the exchange and is currently in a pending state
      if (order.state < Order::Accepted)
        order.state = Order::Accepted;
      LOG_IF(FATAL, has_fill) << "Unexpected!";
      break;
    }
    case roq::common::OrderStatus::Working: {
      // the order is currently in the queue (waiting for matching) -- could be partially filled
      if (order.state < Order::Accepted)
        order.state = Order::Accepted;
      if (is_equal(order.remaining_quantity, 0.0))
        _orders.erase(order_id);
      break;
    }
    case roq::common::OrderStatus::Completed: {
      // the order has been matched and is completely filled
      _orders.erase(order_id);
      break;
    }
    case roq::common::OrderStatus::Cancelled: {
      // the order has been cancelled -- could be partially filled
      _orders.erase(order_id);
      break;
    }
  }
}

// utilities

void OrderManager::add_timeout_check(uint32_t order_id) {
  _timeout.emplace_back(_last_update_time + _config.order_timeout, order_id);
}

void OrderManager::check(const roq::common::MessageInfo& message_info) {
  // all events contain a timestamp -- this check will catch programming mistakes
  auto current_time = message_info.client_receive_time;
  LOG_IF(FATAL, _last_update_time < current_time) << "Wrong sequencing!";
  _last_update_time = current_time;
  // check timeouts
  while (!_timeout.empty()) {
    const auto& front = _timeout.front();
    // the list is increasing -- we can safely stop when a timeout is in the future
    if (current_time < front.first)
      break;
    auto order_id = front.second;
    auto iter = _orders.find(order_id);
    if (iter != _orders.end()) {
      auto& order = (*iter).second;
      if (order.state == Order::Deleting) {
        // this could be wrong! only way to find out is to check trade events...
        _exposure.update(Exposure::Cancel, order.direction, order.remaining_quantity);
      }
      _orders.erase(iter);
    }
    _timeout.pop_front();
  }
}

}  // namespace reference
}  // namespace examples
