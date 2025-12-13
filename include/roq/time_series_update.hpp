/* Copyright (c) 2017-2026, Hans Erik Thrane */

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
#include "roq/data_source.hpp"
#include "roq/event.hpp"
#include "roq/interval.hpp"
#include "roq/name.hpp"
#include "roq/origin.hpp"
#include "roq/trace.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Update relating to time-series
struct ROQ_PUBLIC TimeSeriesUpdate final {
  uint16_t stream_id = {};                          //!< Stream identifier
  std::string_view exchange;                        //!< Exchange
  std::string_view symbol;                          //!< Symbol
  roq::DataSource data_source = {};                 //!< Data source
  roq::Interval interval = {};                      //!< Update frequency
  roq::Origin origin = {};                          //!< Origin of time-series
  std::span<roq::Bar const> bars;                   //!< List of updated bars
  roq::UpdateType update_type = {};                 //!< Update type
  std::chrono::nanoseconds exchange_time_utc = {};  //!< Exchange timestamp, possibly from matching engine (UTC)
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
        R"(data_source={}, )"
        R"(interval={}, )"
        R"(origin={}, )"
        R"(bars=[{}], )"
        R"(update_type={}, )"
        R"(exchange_time_utc={})"
        R"(}})"sv,
        value.stream_id,
        value.exchange,
        value.symbol,
        value.data_source,
        value.interval,
        value.origin,
        fmt::join(value.bars, ", "sv),
        value.update_type,
        value.exchange_time_utc);
  }
};