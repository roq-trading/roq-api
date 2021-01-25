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

#include "roq/fill.h"
#include "roq/position_effect.h"
#include "roq/side.h"

namespace roq {

//! Update relating to order being partially or fully filled
struct ROQ_PUBLIC TradeUpdate final {
  TradeUpdate() = default;
  TradeUpdate(TradeUpdate &&) = default;
  TradeUpdate(const TradeUpdate &) = delete;

  std::string_view account;     //!< Account name (as known to the gateway)
  uint32_t order_id = 0;        //!< Order identifier (as known to client)
  std::string_view exchange;    //!< Exchange name
  std::string_view symbol;      //!< Symbol
  Side side = Side::UNDEFINED;  //!< Side
  PositionEffect position_effect = PositionEffect::UNDEFINED;  //!< Position effect
  std::string_view order_template;                //!< Order template (as known to the gateway)
  std::chrono::nanoseconds create_time_utc = {};  //!< Created timestamp (UTC)
  std::chrono::nanoseconds update_time_utc = {};  //!< Updated timestamp (UTC)
  uint32_t gateway_order_id = 0;                  //!< Order identifier (as known to gateway)
  std::string_view external_account;  //!< External account name (as known to broker or exchange)
  std::string_view
      external_order_id;  //!< External order identifier (as known to broker or exchange)
  roq::span<Fill> fills;  //!< List of fills
};

}  // namespace roq

template <>
struct fmt::formatter<roq::TradeUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::TradeUpdate &value, Context &context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(account="{}", )"
        R"(order_id={}, )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(side={}, )"
        R"(position_effect={}, )"
        R"(order_template="{}", )"
        R"(create_time_utc={}, )"
        R"(update_time_utc={}, )"
        R"(gateway_order_id={}, )"
        R"(external_account="{}", )"
        R"(external_order_id="{}", )"
        R"(fills=[{}])"
        R"(}})",
        value.account,
        value.order_id,
        value.exchange,
        value.symbol,
        value.side,
        value.position_effect,
        value.order_template,
        value.create_time_utc,
        value.update_time_utc,
        value.gateway_order_id,
        value.external_account,
        value.external_order_id,
        fmt::join(value.fills, ", "));
  }
};
template <>
struct fmt::formatter<roq::Event<roq::TradeUpdate> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Event<roq::TradeUpdate> &event, Context &context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(trade_update={})"
        R"(}})",
        event.message_info,
        event.value);
  }
};
