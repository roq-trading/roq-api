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

//! Custom metrics (publish)
struct ROQ_PUBLIC CustomMetrics final {
  std::string_view label;               //!< Label
  std::string_view account;             //!< Account name
  std::string_view exchange;            //!< Exchange
  std::string_view symbol;              //!< Symbol
  std::span<Measurement> measurements;  //!< List of measurements
};

}  // namespace roq

template <>
struct fmt::formatter<roq::CustomMetrics> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::CustomMetrics &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(label="{}", )"
        R"(account="{}", )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(measurements=[{}])"
        R"(}})"sv,
        value.label,
        value.account,
        value.exchange,
        value.symbol,
        fmt::join(value.measurements, ", "sv));
  }
};
template <>
struct fmt::formatter<roq::Event<roq::CustomMetrics> > {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::Event<roq::CustomMetrics> &event, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(custom_metrics={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
