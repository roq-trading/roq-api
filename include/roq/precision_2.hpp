/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <limits>

#include "roq/precision.hpp"

namespace roq {

//! Numerical precision
// XXX TODO rename
struct ROQ_PUBLIC Precision2 final {
  double increment = std::numeric_limits<double>::quiet_NaN();
  Precision precision = {};
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Precision2> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Precision2 const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(increment={}, )"
        R"(precision={})"
        R"(}})"sv,
        value.increment,
        value.precision);
  }
};
