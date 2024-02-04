/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/precision.hpp"

#include "roq/utils/common.hpp"

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
    using namespace std::literals;
    auto decimal_digits = roq::utils::decimal_digits(value.precision);
    if (decimal_digits >= 0)
      return fmt::format_to(context.out(), "{:.{}f}"sv, value.value, decimal_digits);
    return fmt::format_to(context.out(), "{}"sv, value.value);
  }
};
