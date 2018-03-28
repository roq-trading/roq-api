/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

namespace examples {
namespace reference {

class Order {
 public:
  Order(roq::common::TradeDirection direction, double quantity)
      : direction(direction), remaining_quantity(quantity) {}
  const roq::common::TradeDirection direction;
  enum { Requested, Sent, Accepted, Deleting } state = Requested;
  double remaining_quantity;
  uint32_t gateway_order_id = 0;
  double fill(const roq::common::OrderUpdate& order_update);
 private:
  double _traded_quantity = 0.0;
};

}  // namespace reference
}  // namespace examples

