/* Copyright (c) 2017-2026, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/limits.hpp"
#include "roq/precision.hpp"

namespace roq {

struct Decimal final {
  Decimal() = default;

  Decimal(double value, Precision precision) : value_{value}, precision_{precision} {}

  bool empty() const { return std::isnan(value_); }

  operator double() const { return value_; }
  operator Precision() const { return precision_; }

 private:
  double value_ = NaN;
  Precision precision_ = {};
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Decimal> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Decimal const &value, format_context &context) const {
    constexpr auto helper = [](auto precision) {
      using result_type = int8_t;
      if (precision >= roq::Precision::_0 && precision <= roq::Precision::_15) {
        return static_cast<result_type>(precision) - static_cast<result_type>(roq::Precision::_0);
      }
      return -1;
    };
    // NOLINTBEGIN(readability-magic-numbers)
    static_assert(helper(roq::Precision::UNDEFINED) == -1);
    static_assert(helper(roq::Precision::_0) == 0);
    static_assert(helper(roq::Precision::_15) == 15);
    // NOLINTEND(readability-magic-numbers)
    using namespace std::literals;
    auto decimal_digits = helper(static_cast<roq::Precision>(value));
    if (decimal_digits >= 0) {
      return fmt::format_to(context.out(), "{:.{}f}"sv, static_cast<double>(value), decimal_digits);
    }
    return fmt::format_to(context.out(), "{}"sv, static_cast<double>(value));
  }
};
