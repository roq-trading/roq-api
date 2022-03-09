/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/compat.hpp"
#include "roq/event.hpp"
#include "roq/message_info.hpp"
#include "roq/numbers.hpp"
#include "roq/string_buffer.hpp"

#include "roq/measurement.hpp"

namespace roq {

//! Custom metrics (receive)
struct ROQ_PUBLIC CustomMetricsUpdate final {
  std::string_view user;                //!< User name (origin)
  std::string_view label;               //!< Label
  std::string_view account;             //!< Account name
  std::string_view exchange;            //!< Exchange
  std::string_view symbol;              //!< Symbol
  std::span<Measurement> measurements;  //!< List of measurements
};

}  // namespace roq

template <>
struct fmt::formatter<roq::CustomMetricsUpdate> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::CustomMetricsUpdate &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(user="{}", )"
        R"(label="{}", )"
        R"(account="{}", )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(measurements=[{}])"
        R"(}})"sv,
        value.user,
        value.label,
        value.account,
        value.exchange,
        value.symbol,
        fmt::join(value.measurements, ", "sv));
  }
};
template <>
struct fmt::formatter<roq::Event<roq::CustomMetricsUpdate> > {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::Event<roq::CustomMetricsUpdate> &event, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(custom_metrics_update={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
