/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/limits.hpp"
#include "roq/precision.hpp"

namespace roq {

namespace detail {
inline constexpr int8_t decimal_to_digits_helper(Precision precision) {
  switch (precision) {
    using enum Precision;
    case UNDEFINED:
      break;
    case _0:
      return 0;
    case _1:
      return 1;
    case _2:
      return 2;
    case _3:
      return 3;
    case _4:
      return 4;
    case _5:
      return 5;
    case _6:
      return 6;
    case _7:
      return 7;
    case _8:
      return 8;
    case _9:
      return 9;
    case _10:
      return 10;
    case _11:
      return 11;
    case _12:
      return 12;
    case _13:
      return 13;
    case _14:
      return 14;
    case _15:
      return 15;
  }
  return -1;
}
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
    auto decimal_digits = roq::detail::decimal_to_digits_helper(value.precision);
    if (decimal_digits >= 0)
      return fmt::format_to(context.out(), "{:.{}f}"sv, value.value, decimal_digits);
    return fmt::format_to(context.out(), "{}"sv, value.value);
  }
};
