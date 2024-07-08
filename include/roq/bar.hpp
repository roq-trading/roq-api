/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include <chrono>

#include "roq/limits.hpp"

namespace roq {

//! Represents a single bar of a time-series
struct ROQ_PUBLIC Bar final {
  std::chrono::nanoseconds time_utc = {};  //!< End-period time-stamp (UTC)
  double open = roq::NaN;                  //!< Opening price
  double high = roq::NaN;                  //!< Highest price
  double low = roq::NaN;                   //!< Lowest price
  double close = roq::NaN;                 //!< Closing price
  double total_volume = roq::NaN;          //!< Total volume
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
        R"(total_volume={})"
        R"(}})"sv,
        value.time_utc,
        value.open,
        value.high,
        value.low,
        value.close,
        value.total_volume);
  }
};
