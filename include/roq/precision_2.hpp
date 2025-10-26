/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include <magic_enum/magic_enum_format.hpp>

#include "roq/limits.hpp"
#include "roq/precision.hpp"

namespace roq {

//! Numerical precision
// XXX TODO rename
struct ROQ_PUBLIC Precision2 final {
  double increment = NaN;
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
