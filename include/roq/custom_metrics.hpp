/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <span>
#include <string_view>

#include "roq/event.hpp"
#include "roq/measurement.hpp"
#include "roq/name.hpp"
#include "roq/trace.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Custom metrics (publish)
struct ROQ_PUBLIC CustomMetrics final {
  std::string_view label;                          //!< Label
  std::string_view account;                        //!< Account name
  std::string_view exchange;                       //!< Exchange
  std::string_view symbol;                         //!< Symbol
  std::span<roq::Measurement const> measurements;  //!< List of measurements
  roq::UpdateType update_type = {};                //!< Update type
};

template <>
inline constexpr std::string_view get_name<CustomMetrics>() {
  using namespace std::literals;
  return "custom_metrics"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::CustomMetrics> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::CustomMetrics const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(label="{}", )"
        R"(account="{}", )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(measurements=[{}], )"
        R"(update_type={})"
        R"(}})"sv,
        value.label,
        value.account,
        value.exchange,
        value.symbol,
        fmt::join(value.measurements, ", "sv),
        value.update_type);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::CustomMetrics>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::CustomMetrics> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(custom_metrics={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::CustomMetrics>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::CustomMetrics> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(custom_metrics={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
