/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include <magic_enum/magic_enum_format.hpp>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/bar.hpp"
#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/origin.hpp"
#include "roq/time_series_type.hpp"
#include "roq/trace.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Update relating to time-series
struct ROQ_PUBLIC TimeSeriesUpdate final {
  uint16_t stream_id = {};              //!< Stream identifier
  std::string_view exchange;            //!< Exchange
  std::string_view symbol;              //!< Symbol
  roq::Origin origin = {};              //!< Origin of ack
  roq::TimeSeriesType type = {};        //!< Time-series type
  std::chrono::minutes frequency = {};  //!< Frequency
  std::span<roq::Bar const> bars;       //!< List of updated bars
  roq::UpdateType update_type = {};     //!< Update type
};

template <>
constexpr std::string_view get_name<TimeSeriesUpdate>() {
  using namespace std::literals;
  return "time_series_update"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::TimeSeriesUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::TimeSeriesUpdate const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(origin={}, )"
        R"(type={}, )"
        R"(frequency={}, )"
        R"(bars=[{}], )"
        R"(update_type={})"
        R"(}})"sv,
        value.stream_id,
        value.exchange,
        value.symbol,
        value.origin,
        value.type,
        value.frequency,
        fmt::join(value.bars, ", "sv),
        value.update_type);
  }
};
