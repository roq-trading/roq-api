/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/fixed_string.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/span.h"

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
struct fmt::formatter<roq::MarketByPriceUpdate> : public roq::formatter {
  template <typename Context>
  auto format(const roq::MarketByPriceUpdate &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(bids=[{}], )"
        R"(asks=[{}], )"
        R"(snapshot={}, )"
        R"(exchange_time_utc={})"
        R"(}})"_fmt,
        value.exchange,
        value.symbol,
        fmt::join(value.bids, ", "_sv),
        fmt::join(value.asks, ", "_sv),
        value.snapshot,
        value.exchange_time_utc);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::MarketByPriceUpdate> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::MarketByPriceUpdate> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(market_by_price_update={})"
        R"(}})"_fmt,
        event.message_info,
        event.value);
  }
};
