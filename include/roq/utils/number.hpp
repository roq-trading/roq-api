/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/decimals.hpp"

#include "roq/utils/common.hpp"

namespace roq {
namespace utils {

struct Number final {
  Number() = default;
  Number(double value, Decimals decimals) : value{value}, decimals{decimals} {}

  double const value = NaN;
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
    using namespace std::literals;
    auto decimal_digits = roq::utils::decimal_digits(value.decimals);
    if (decimal_digits < 0)
      return fmt::format_to(context.out(), "{}"sv, value.value);
    return fmt::format_to(context.out(), "{:.{}f}"sv, value.value, roq::utils::decimal_digits(value.decimals));
  }
};
