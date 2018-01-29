/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <quinclas/tradingapi.h>

namespace examples {
namespace reference {

// rounding error insensitive number comparison
bool is_equal(double lhs, double rhs);
bool is_less_than_or_equal(double lhs, double rhs);

// trading api helpers
double sign(const quinclas::common::TradeDirection direction);
double signed_quantity(const quinclas::common::TradeDirection direction, const double quantity);

}  // namespace reference
}  // namespace examples
