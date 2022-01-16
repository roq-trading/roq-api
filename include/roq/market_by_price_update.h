/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/compat.h"
#include "roq/event.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/string_buffer.h"

#include "roq/decimals.h"
#include "roq/mbp_update.h"
#include "roq/update_type.h"

namespace roq {

//! Update relating to market by price
struct ROQ_PUBLIC MarketByPriceUpdate final {
  uint16_t stream_id = {};                          //!< Stream identifier
  std::string_view exchange;                        //!< Exchange
  std::string_view symbol;                          //!< Symbol
  std::span<MBPUpdate> bids;                        //!< List of bids
  std::span<MBPUpdate> asks;                        //!< List of asks
  UpdateType update_type = {};                      //!< Update type
  std::chrono::nanoseconds exchange_time_utc = {};  //!< Exchange timestamp (UTC)
  int64_t exchange_sequence = {};                   //!< Latest sequence number (from exchange)
  Decimals price_decimals = {};                     //!< Decimal digits required to represent prices
  Decimals quantity_decimals = {};                  //!< Decimal digits required to represent quantities
  uint16_t max_depth = {};                          //!< Maximum depth (used to maintain a view of top N price levels)
  uint32_t checksum = {};                           //!< Checksum (internal)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::MarketByPriceUpdate> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::MarketByPriceUpdate &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(bids=[{}], )"
        R"(asks=[{}], )"
        R"(update_type={}, )"
        R"(exchange_time_utc={}, )"
        R"(exchange_sequence={}, )"
        R"(price_decimals={}, )"
        R"(quantity_decimals={}, )"
        R"(max_depth={}, )"
        R"(checksum={})"
        R"(}})"sv,
        value.stream_id,
        value.exchange,
        value.symbol,
        fmt::join(value.bids, ", "sv),
        fmt::join(value.asks, ", "sv),
        value.update_type,
        value.exchange_time_utc,
        value.exchange_sequence,
        value.price_decimals,
        value.quantity_decimals,
        value.max_depth,
        value.checksum);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::MarketByPriceUpdate> > {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::Event<roq::MarketByPriceUpdate> &event, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(market_by_price_update={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
