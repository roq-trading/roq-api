/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/order.h"
#include <roq/logging.h>
#include <algorithm>

namespace examples {
namespace reference {

double Order::fill(const roq::common::OrderUpdate& order_update) {
  // FIXME(thraneh): this can be done in a safer manner
  auto result = std::max(order_update.traded_quantity - _traded_quantity, 0.0);
  _traded_quantity = order_update.traded_quantity;
  return result;
}

}  // namespace reference
}  // namespace examples
