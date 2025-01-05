/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include <fmt/core.h>

#include "roq/limits.hpp"
#include "roq/precision.hpp"

namespace roq {

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
    constexpr auto helper = [](auto precision) {
      using result_type = int8_t;
      if (precision >= roq::Precision::_0 && precision <= roq::Precision::_15)
        return static_cast<result_type>(precision) - static_cast<result_type>(roq::Precision::_0);
      return -1;
    };
    static_assert(helper(roq::Precision::UNDEFINED) == -1);
    static_assert(helper(roq::Precision::_0) == 0);
    static_assert(helper(roq::Precision::_15) == 15);
    using namespace std::literals;
    auto decimal_digits = helper(value.precision);
    if (decimal_digits >= 0)
      return fmt::format_to(context.out(), "{:.{}f}"sv, value.value, decimal_digits);
    return fmt::format_to(context.out(), "{}"sv, value.value);
  }
};
