/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/fixed_string.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"

#include "roq/trading_status.h"

namespace roq {

//! Update relating to current trading status of a symbol
struct ROQ_PUBLIC MarketStatus final {
  uint16_t stream_id = {};                                  //!< Stream identifier
  std::string_view exchange;                                //!< Exchange name
  std::string_view symbol;                                  //!< Symbol
  TradingStatus trading_status = TradingStatus::UNDEFINED;  //!< Trading status
};

}  // namespace roq

template <>
struct fmt::formatter<roq::MarketStatus> : public roq::formatter {
  template <typename Context>
  auto format(const roq::MarketStatus &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(trading_status={})"
        R"(}})"_fmt,
        value.stream_id,
        value.exchange,
        value.symbol,
        value.trading_status);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::MarketStatus> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::MarketStatus> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(market_status={})"
        R"(}})"_fmt,
        event.message_info,
        event.value);
  }
};
