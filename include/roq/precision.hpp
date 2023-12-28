/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include <limits>

#include "roq/decimals.hpp"

namespace roq {

//! Numerical precision
struct ROQ_PUBLIC Precision final {
  double increment = std::numeric_limits<double>::quiet_NaN();
  Decimals decimals = {};
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Precision> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Precision const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(increment={}, )"
        R"(decimals={})"
        R"(}})"_cf,
        value.increment,
        value.decimals);
  }
};
