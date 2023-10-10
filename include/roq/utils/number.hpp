/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
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
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::utils::Number const &value, Context &context) const {
    using namespace fmt::literals;
    auto decimal_digits = roq::utils::decimal_digits(value.decimals);
    if (decimal_digits >= 0)
      return fmt::format_to(context.out(), "{:.{}f}"_cf, value.value, decimal_digits);
    return fmt::format_to(context.out(), "{}"_cf, value.value);
  }
};
