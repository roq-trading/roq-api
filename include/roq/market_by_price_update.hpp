/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/event.hpp"
#include "roq/mbp_update.hpp"
#include "roq/name.hpp"
#include "roq/precision.hpp"
#include "roq/trace.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Update relating to market by price
struct ROQ_PUBLIC MarketByPriceUpdate final {
  uint16_t stream_id = {};                          //!< Stream identifier
  std::string_view exchange;                        //!< Exchange
  std::string_view symbol;                          //!< Symbol
  std::span<roq::MBPUpdate const> bids;             //!< List of bids
  std::span<roq::MBPUpdate const> asks;             //!< List of asks
  roq::UpdateType update_type = {};                 //!< Update type
  std::chrono::nanoseconds exchange_time_utc = {};  //!< Exchange timestamp, possibly from matching engine (UTC)
  uint64_t exchange_sequence = {};                  //!< Exchange message sequence number
  std::chrono::nanoseconds sending_time_utc = {};   //!< Exchange sending timestamp (UTC)
  roq::Precision price_precision = {};     //!< Precision (decimal digits) required to represent prices (dynamic)
  roq::Precision quantity_precision = {};  //!< Precision (decimal digits) required to represent quantities (dynamic)
  uint16_t max_depth = {};                 //!< Maximum depth (zero means unlimited)
  uint32_t checksum = {};                  //!< Checksum (internal)
};

template <>
inline constexpr std::string_view get_name<MarketByPriceUpdate>() {
  using namespace std::literals;
  return "market_by_price_update"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::MarketByPriceUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::MarketByPriceUpdate const &value, format_context &context) const {
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
        R"(sending_time_utc={}, )"
        R"(price_precision={}, )"
        R"(quantity_precision={}, )"
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
        value.sending_time_utc,
        value.price_precision,
        value.quantity_precision,
        value.max_depth,
        value.checksum);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::MarketByPriceUpdate>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::MarketByPriceUpdate> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(market_by_price_update={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::MarketByPriceUpdate>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::MarketByPriceUpdate> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(market_by_price_update={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
