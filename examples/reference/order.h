/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <quinclas/tradingapi.h>

namespace examples {
namespace reference {

class Order {
 public:
  Order(quinclas::common::TradeDirection direction, double quantity)
      : direction(direction), remaining_quantity(quantity) {}
  const quinclas::common::TradeDirection direction;
  enum { Requested, Sent, Working } state = Requested;
  double remaining_quantity;
  uint32_t gateway_order_id = 0;
};

}  // namespace reference
}  // namespace examples

