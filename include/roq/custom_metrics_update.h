/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/string_buffer.h"

#include "roq/measurement.h"

namespace roq {

//! Custom metrics (receive)
struct ROQ_PUBLIC CustomMetricsUpdate final {
  std::string_view user;                //!< User name (origin)
  std::string_view label;               //!< Label
  std::string_view account;             //!< Account name
  std::string_view exchange;            //!< Exchange
  std::string_view symbol;              //!< Symbol
  roq::span<Measurement> measurements;  //!< List of measurements
};

}  // namespace roq

template <>
struct fmt::formatter<roq::CustomMetricsUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::CustomMetricsUpdate &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(user="{}", )"
        R"(label="{}", )"
        R"(account="{}", )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(measurements=[{}])"
        R"(}})"_sv,
        value.user,
        value.label,
        value.account,
        value.exchange,
        value.symbol,
        fmt::join(value.measurements, ", "_sv));
  }
};
template <>
struct fmt::formatter<roq::Event<roq::CustomMetricsUpdate> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Event<roq::CustomMetricsUpdate> &event, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(custom_metrics_update={})"
        R"(}})"_sv,
        event.message_info,
        event.value);
  }
};
