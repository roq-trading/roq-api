/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

namespace examples {
namespace reference {

// rounding error insensitive number comparison
bool is_equal(double lhs, double rhs);
bool is_less_than_or_equal(double lhs, double rhs);

// trading api helpers
double sign(const roq::common::TradeDirection direction);
double signed_quantity(const roq::common::TradeDirection direction, const double quantity);

}  // namespace reference
}  // namespace examples
