/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <magic_enum/magic_enum_format.hpp>

#include <chrono>

#include "roq/limits.hpp"

namespace roq {

//! Represents a single bar of a time-series
struct ROQ_PUBLIC Bar final {
  std::chrono::nanoseconds time_utc = {};  //!< End-period time-stamp (UTC)
  double open = roq::NaN;                  //!< Open
  double high = roq::NaN;                  //!< High
  double low = roq::NaN;                   //!< Low
  double close = roq::NaN;                 //!< Close
  double volume = roq::NaN;                //!< Volume
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Bar> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Bar const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(time_utc={}, )"
        R"(open={}, )"
        R"(high={}, )"
        R"(low={}, )"
        R"(close={}, )"
        R"(volume={})"
        R"(}})"sv,
        value.time_utc,
        value.open,
        value.high,
        value.low,
        value.close,
        value.volume);
  }
};
