/* Copyright (c) 2017-2023, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/compile.h>
#include <fmt/format.h>

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
  std::span<Measurement const> measurements;       //!< List of measurements
  UpdateType update_type = {};                     //!< Update type
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
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::CustomMetricsUpdate const &value, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
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
        R"(}})"_cf,
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

template <>
struct fmt::formatter<roq::Event<roq::CustomMetricsUpdate>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Event<roq::CustomMetricsUpdate> const &event, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(custom_metrics_update={}, )"
        R"(message_info={})"
        R"(}})"_cf,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::CustomMetricsUpdate>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Trace<roq::CustomMetricsUpdate> const &event, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(custom_metrics_update={}, )"
        R"(trace_info={})"
        R"(}})"_cf,
        event.value,
        event.trace_info);
  }
};
