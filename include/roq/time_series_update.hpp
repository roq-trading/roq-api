/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/core.h>
#include <fmt/ranges.h>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/bar.hpp"
#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/precision.hpp"
#include "roq/time_series_type.hpp"
#include "roq/trace.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Update relating to time-series
struct ROQ_PUBLIC TimeSeriesUpdate final {
  uint16_t stream_id = {};               //!< Stream identifier
  std::string_view exchange;             //!< Exchange
  std::string_view symbol;               //!< Symbol
  roq::TimeSeriesType type = {};         //!< Time-series type
  std::chrono::minutes frequency = {};   //!< Frequency
  std::span<roq::Bar const> bars;        //!< List of updated bars
  roq::UpdateType update_type = {};      //!< Update type
  roq::Precision value_precision = {};   //!< Precision (decimal digits) required to represent values (dynamic)
  roq::Precision volume_precision = {};  //!< Precision (decimal digits) required to represent volume (dynamic)
};

template <>
inline constexpr std::string_view get_name<TimeSeriesUpdate>() {
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
        R"(type={}, )"
        R"(frequency={}, )"
        R"(bars=[{}], )"
        R"(update_type={}, )"
        R"(value_precision={}, )"
        R"(volume_precision={})"
        R"(}})"sv,
        value.stream_id,
        value.exchange,
        value.symbol,
        value.type,
        value.frequency,
        fmt::join(value.bars, ", "sv),
        value.update_type,
        value.value_precision,
        value.volume_precision);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::TimeSeriesUpdate>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::TimeSeriesUpdate> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(time_series_update={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::TimeSeriesUpdate>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::TimeSeriesUpdate> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(time_series_update={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
