/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include <magic_enum/magic_enum_format.hpp>

#include "roq/limits.hpp"

namespace roq {

//! Represents tick size for a range starting from a minimum price
struct ROQ_PUBLIC TickSizeStep final {
  double min_price = roq::NaN;  //!< Minimum price (of range)
  double tick_size = roq::NaN;  //!< Tick size
};

}  // namespace roq

template <>
struct fmt::formatter<roq::TickSizeStep> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::TickSizeStep const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(min_price={}, )"
        R"(tick_size={})"
        R"(}})"sv,
        value.min_price,
        value.tick_size);
  }
};