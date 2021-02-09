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

#include "roq/mbo_update.h"

namespace roq {

//! Update relating to market by order
struct ROQ_PUBLIC MarketByOrderUpdate final {
  MarketByOrderUpdate() = default;
  MarketByOrderUpdate(MarketByOrderUpdate &&) = default;
  MarketByOrderUpdate(const MarketByOrderUpdate &) = delete;

  std::string_view exchange;  //!< Exchange name
  std::string_view symbol;    //!< Symbol
  roq::span<MBOUpdate> bids;  //!< List of bids
  roq::span<MBOUpdate> asks;  //!< List of asks
  bool snapshot =
      false;  //!< Full update (possibly old) if true and otherwise an incremental update
  std::chrono::nanoseconds exchange_time_utc = {};  //!< Exchange timestamp (UTC)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::MarketByOrderUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::MarketByOrderUpdate &value, Context &context) {
    using namespace std::literals;  // NOLINT
    return format_to(
        context.out(),
        R"({{)"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(bids=[{}], )"
        R"(asks=[{}], )"
        R"(snapshot={}, )"
        R"(exchange_time_utc={})"
        R"(}})"sv,
        value.exchange,
        value.symbol,
        fmt::join(value.bids, ", "),
        fmt::join(value.asks, ", "),
        value.snapshot,
        value.exchange_time_utc);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::MarketByOrderUpdate> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Event<roq::MarketByOrderUpdate> &event, Context &context) {
    using namespace std::literals;  // NOLINT
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(market_by_order_update={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
