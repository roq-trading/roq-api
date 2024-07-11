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
#include "roq/trace.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Update relating to time-series
struct ROQ_PUBLIC TimeSeriesUpdate final {
  uint16_t stream_id = {};                 //!< Stream identifier
  std::string_view exchange;               //!< Exchange
  std::string_view symbol;                 //!< Symbol
  std::chrono::minutes frequency = {};     //!< Frequency
  std::span<roq::Bar const> bars;          //!< List of updated bars
  roq::UpdateType update_type = {};        //!< Update type
  roq::Precision price_precision = {};     //!< Precision (decimal digits) required to represent prices (dynamic)
  roq::Precision quantity_precision = {};  //!< Precision (decimal digits) required to represent quantities (dynamic)
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
        R"(frequency={}, )"
        R"(bars=[{}], )"
        R"(update_type={}, )"
        R"(price_precision={}, )"
        R"(quantity_precision={})"
        R"(}})"sv,
        value.stream_id,
        value.exchange,
        value.symbol,
        value.frequency,
        fmt::join(value.bars, ", "sv),
        value.update_type,
        value.price_precision,
        value.quantity_precision);
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
