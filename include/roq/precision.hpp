/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

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
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Precision const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(increment={}, )"
        R"(decimals={})"
        R"(}})"sv,
        value.increment,
        value.decimals);
  }
};
