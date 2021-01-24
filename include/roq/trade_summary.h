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

#include "roq/trade.h"

namespace roq {

//! Update relating to trade reporting by the exchange
struct ROQ_PUBLIC TradeSummary final {
  TradeSummary() = default;
  TradeSummary(TradeSummary &&) = default;
  TradeSummary(const TradeSummary &) = delete;

  std::string_view exchange;                        //!< Exchange name
  std::string_view symbol;                          //!< Symbol
  roq::span<Trade> trades;                          //!< List of trades
  std::chrono::nanoseconds exchange_time_utc = {};  //!< Exchange timestamp (UTC)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::TradeSummary> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::TradeSummary &value, Context &context) {
    return format_to(
        context.out(),

        R"({{)"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(trades=[{}], )"
        R"(exchange_time_utc={})"

        R"(}})",
        value.exchange,
        value.symbol,
        fmt::join(value.trades, ", "),
        value.exchange_time_utc);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::TradeSummary> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Event<roq::TradeSummary> &event, Context &context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(trade_summary={})"
        R"(}})",
        event.message_info,
        event.value);
  }
};
