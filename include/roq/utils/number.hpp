/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/decimals.hpp"

#include "roq/utils/common.hpp"
#include "roq/utils/compare.hpp"

namespace roq {
namespace utils {

struct Number final {
  using value_type = double;

  Number() = default;
  Number(value_type value, Decimals decimals) : value{value}, decimals{decimals} {}

  bool empty() const { return std::isnan(value); }

  value_type const value = NaN;
  Decimals const decimals = {};
};

}  // namespace utils
}  // namespace roq

template <>
struct fmt::formatter<roq::utils::Number> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::utils::Number const &value, format_context &context) const {
    using namespace std::literals;
    auto decimal_digits = roq::utils::decimal_digits(value.decimals);
    if (decimal_digits >= 0)
      return fmt::format_to(context.out(), "{:.{}f}"sv, value.value, decimal_digits);
    return fmt::format_to(context.out(), "{}"sv, value.value);
  }
};
