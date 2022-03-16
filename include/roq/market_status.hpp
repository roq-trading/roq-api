/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/compat.hpp"
#include "roq/event.hpp"
#include "roq/message_info.hpp"
#include "roq/numbers.hpp"
#include "roq/string.hpp"

#include "roq/trading_status.hpp"

namespace roq {

//! Update relating to current trading status of a symbol
struct ROQ_PUBLIC MarketStatus final {
  uint16_t stream_id = {};            //!< Stream identifier
  std::string_view exchange;          //!< Exchange
  std::string_view symbol;            //!< Symbol
  TradingStatus trading_status = {};  //!< Trading status
};

}  // namespace roq

template <>
struct fmt::formatter<roq::MarketStatus> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::MarketStatus &value, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(trading_status={})"
        R"(}})"sv,
        value.stream_id,
        value.exchange,
        value.symbol,
        value.trading_status);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::MarketStatus> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::Event<roq::MarketStatus> &event, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(market_status={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
