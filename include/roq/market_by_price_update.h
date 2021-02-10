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

#include "roq/mbp_update.h"

namespace roq {

//! Update relating to market by price
struct ROQ_PUBLIC MarketByPriceUpdate final {
  MarketByPriceUpdate() = default;
  MarketByPriceUpdate(MarketByPriceUpdate &&) = default;
  MarketByPriceUpdate(const MarketByPriceUpdate &) = delete;

  std::string_view exchange;  //!< Exchange name
  std::string_view symbol;    //!< Symbol
  roq::span<MBPUpdate> bids;  //!< List of bids
  roq::span<MBPUpdate> asks;  //!< List of asks
  bool snapshot =
      false;  //!< Full update (possibly old) if true and otherwise an incremental update
  std::chrono::nanoseconds exchange_time_utc = {};  //!< Exchange timestamp (UTC)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::MarketByPriceUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::MarketByPriceUpdate &value, Context &context) {
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
        fmt::join(value.bids, ", "sv),
        fmt::join(value.asks, ", "sv),
        value.snapshot,
        value.exchange_time_utc);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::MarketByPriceUpdate> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Event<roq::MarketByPriceUpdate> &event, Context &context) {
    using namespace std::literals;  // NOLINT
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(market_by_price_update={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
