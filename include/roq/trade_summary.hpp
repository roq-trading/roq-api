/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include <magic_enum/magic_enum_format.hpp>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/precision.hpp"
#include "roq/trace.hpp"
#include "roq/trade.hpp"

namespace roq {

//! Update relating to trade reporting by the exchange
struct ROQ_PUBLIC TradeSummary final {
  uint16_t stream_id = {};                          //!< Stream identifier
  std::string_view exchange;                        //!< Exchange
  std::string_view symbol;                          //!< Symbol
  std::span<roq::Trade const> trades;               //!< List of trades
  std::chrono::nanoseconds exchange_time_utc = {};  //!< Exchange timestamp, possibly from matching engine (UTC)
  uint64_t exchange_sequence = {};                  //!< Exchange message sequence number
  std::chrono::nanoseconds sending_time_utc = {};   //!< Exchange sending timestamp (UTC)
  roq::Precision price_precision = {};              //!< Precision (decimal digits) required to represent prices (dynamic)
  roq::Precision quantity_precision = {};           //!< Precision (decimal digits) required to represent quantities (dynamic)
};

template <>
constexpr std::string_view get_name<TradeSummary>() {
  using namespace std::literals;
  return "trade_summary"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::TradeSummary> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::TradeSummary const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(trades=[{}], )"
        R"(exchange_time_utc={}, )"
        R"(exchange_sequence={}, )"
        R"(sending_time_utc={}, )"
        R"(price_precision={}, )"
        R"(quantity_precision={})"
        R"(}})"sv,
        value.stream_id,
        value.exchange,
        value.symbol,
        fmt::join(value.trades, ", "sv),
        value.exchange_time_utc,
        value.exchange_sequence,
        value.sending_time_utc,
        value.price_precision,
        value.quantity_precision);
  }
};