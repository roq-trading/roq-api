/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <magic_enum/magic_enum_format.hpp>

#include <chrono>
#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/trace.hpp"
#include "roq/trading_status.hpp"

namespace roq {

//! Update relating to current trading status of a symbol
struct ROQ_PUBLIC MarketStatus final {
  uint16_t stream_id = {};                          //!< Stream identifier
  std::string_view exchange;                        //!< Exchange
  std::string_view symbol;                          //!< Symbol
  roq::TradingStatus trading_status = {};           //!< Trading status
  std::chrono::nanoseconds exchange_time_utc = {};  //!< Exchange timestamp, possibly from matching engine (UTC)
  uint64_t exchange_sequence = {};                  //!< Exchange message sequence number
  std::chrono::nanoseconds sending_time_utc = {};   //!< Exchange sending timestamp (UTC)
};

template <>
constexpr std::string_view get_name<MarketStatus>() {
  using namespace std::literals;
  return "market_status"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::MarketStatus> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::MarketStatus const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(trading_status={}, )"
        R"(exchange_time_utc={}, )"
        R"(exchange_sequence={}, )"
        R"(sending_time_utc={})"
        R"(}})"sv,
        value.stream_id,
        value.exchange,
        value.symbol,
        value.trading_status,
        value.exchange_time_utc,
        value.exchange_sequence,
        value.sending_time_utc);
  }
};