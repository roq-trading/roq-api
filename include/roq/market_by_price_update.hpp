/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/compat.hpp"
#include "roq/event.hpp"
#include "roq/mask.hpp"
#include "roq/message_info.hpp"
#include "roq/name.hpp"
#include "roq/numbers.hpp"
#include "roq/span.hpp"
#include "roq/string_types.hpp"
#include "roq/trace.hpp"

#include "roq/decimals.hpp"
#include "roq/mbp_update.hpp"
#include "roq/update_type.hpp"

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

template <>
inline constexpr std::string_view get_name<MarketByPriceUpdate>() {
  using namespace std::literals;
  return "market_by_price_update"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::MarketByPriceUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::MarketByPriceUpdate &value, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
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
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::Event<roq::MarketByPriceUpdate> &event, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(market_by_price_update={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::MarketByPriceUpdate const> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::Trace<roq::MarketByPriceUpdate const> &event, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(trace_info={}, )"
        R"(market_by_price_update={})"
        R"(}})"sv,
        event.trace_info,
        event.value);
  }
};
