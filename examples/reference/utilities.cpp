/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/utilities.h"
#include <quinclas/logging.h>
#include <cmath>

namespace examples {
namespace reference {

const double TOLERANCE = 1.0e-8;

bool is_equal(double lhs, double rhs) {
  return std::fabs(lhs - rhs) < TOLERANCE;
}

bool is_less_than_or_equal(double lhs, double rhs) {
  return lhs <= (rhs + TOLERANCE);
}

double sign(const quinclas::common::TradeDirection direction) {
  switch (direction) {
    case quinclas::common::TradeDirection::Buy:
      return 1.0;
    case quinclas::common::TradeDirection::Sell:
      return -1.0;
    default:
      LOG(FATAL) << "Received unknown trade direction!";
      std::abort();  // FIXME(thraneh): avoid compiler warnings until LOG(FATAL) has been fixed
  }
}

double signed_quantity(const quinclas::common::TradeDirection direction, const double quantity) {
  assert(quantity >= 0.0);
  return sign(direction) * quantity;
}

}  // namespace reference
}  // namespace examples
