/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/core.h>
#include <fmt/ranges.h>

#include <magic_enum/magic_enum_format.hpp>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/event.hpp"
#include "roq/measurement.hpp"
#include "roq/name.hpp"
#include "roq/trace.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Custom metrics (receive)
struct ROQ_PUBLIC CustomMetricsUpdate final {
  std::string_view label;                          //!< Label
  std::string_view account;                        //!< Account name
  std::string_view exchange;                       //!< Exchange
  std::string_view symbol;                         //!< Symbol
  std::span<roq::Measurement const> measurements;  //!< List of measurements
  roq::UpdateType update_type = {};                //!< Update type
  std::chrono::nanoseconds sending_time_utc = {};  //!< Exchange sending timestamp (UTC)
  std::string_view user;                           //!< User name (origin)
};

template <>
inline constexpr std::string_view get_name<CustomMetricsUpdate>() {
  using namespace std::literals;
  return "custom_metrics_update"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::CustomMetricsUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::CustomMetricsUpdate const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(label="{}", )"
        R"(account="{}", )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(measurements=[{}], )"
        R"(update_type={}, )"
        R"(sending_time_utc={}, )"
        R"(user="{}")"
        R"(}})"sv,
        value.label,
        value.account,
        value.exchange,
        value.symbol,
        fmt::join(value.measurements, ", "sv),
        value.update_type,
        value.sending_time_utc,
        value.user);
  }
};
