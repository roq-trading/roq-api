/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/compat.h"
#include "roq/fixed_string.h"
#include "roq/span.h"

#include "roq/event.h"
#include "roq/message_info.h"

#include "roq/statistics.h"

namespace roq {

//! Update relating to statistics published by the exchange
struct ROQ_PUBLIC StatisticsUpdate final {
  StatisticsUpdate() = default;
  StatisticsUpdate(StatisticsUpdate &&) = default;
  StatisticsUpdate(const StatisticsUpdate &) = delete;

  std::string_view exchange;         //!< Exchange name
  std::string_view symbol;           //!< Symbol
  roq::span<Statistics> statistics;  //!< List of statistics
  bool snapshot =
      false;  //!< Full update (possibly old) if true and otherwise an incremental update
  std::chrono::nanoseconds exchange_time_utc = {};  //!< Exchange timestamp (UTC)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::StatisticsUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::StatisticsUpdate &value, Context &context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(statistics=[{}], )"
        R"(snapshot={}, )"
        R"(exchange_time_utc={})"
        R"(}})",
        value.exchange,
        value.symbol,
        fmt::join(value.statistics, ", "),
        value.snapshot,
        value.exchange_time_utc);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::StatisticsUpdate> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Event<roq::StatisticsUpdate> &event, Context &context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(statistics_update={})"
        R"(}})",
        event.message_info,
        event.value);
  }
};
