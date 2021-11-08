/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/decimals.h"

#include "roq/utils/common.h"

namespace roq {
namespace utils {

struct Number final {
  Number() = default;
  Number(double value, Decimals decimals) : value(value), decimals(decimals) {}

  const double value = NaN;
  const Decimals decimals = {};
};

}  // namespace utils
}  // namespace roq

template <>
struct fmt::formatter<roq::utils::Number> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::utils::Number &value, Context &context) {
    using namespace std::literals;
    auto decimal_digits = roq::utils::decimal_digits(value.decimals);
    if (decimal_digits < 0)
      return fmt::format_to(context.out(), "{}"sv, value.value);
    return fmt::format_to(context.out(), "{:.{}f}"sv, value.value, roq::utils::decimal_digits(value.decimals));
  }
};
