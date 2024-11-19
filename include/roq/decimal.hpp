/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/limits.hpp"
#include "roq/precision.hpp"

namespace roq {

namespace detail {
inline constexpr auto precision_to_decimal_digits_helper(Precision precision) {
  using result_type = int8_t;
  if (precision >= Precision::_0 && precision <= Precision::_15)
    return static_cast<result_type>(precision) - static_cast<result_type>(Precision::_0);
  return -1;
}
static_assert(precision_to_decimal_digits_helper(Precision::UNDEFINED) == -1);
static_assert(precision_to_decimal_digits_helper(Precision::_0) == 0);
static_assert(precision_to_decimal_digits_helper(Precision::_15) == 15);
}  // namespace detail

struct Decimal final {
  using value_type = double;

  Decimal() = default;

  Decimal(value_type value, Precision precision) : value{value}, precision{precision} {}

  bool empty() const { return std::isnan(value); }

  value_type const value = NaN;
  Precision const precision = {};
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Decimal> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Decimal const &value, format_context &context) const {
    using namespace std::literals;
    auto decimal_digits = roq::detail::precision_to_decimal_digits_helper(value.precision);
    if (decimal_digits >= 0)
      return fmt::format_to(context.out(), "{:.{}f}"sv, value.value, decimal_digits);
    return fmt::format_to(context.out(), "{}"sv, value.value);
  }
};
