/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/core.h>
#include <fmt/ranges.h>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/event.hpp"
#include "roq/fill.hpp"
#include "roq/margin_mode.hpp"
#include "roq/name.hpp"
#include "roq/position_effect.hpp"
#include "roq/quantity_type.hpp"
#include "roq/side.hpp"
#include "roq/trace.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Update relating to order being partially or fully filled
struct ROQ_PUBLIC TradeUpdate final {
  uint16_t stream_id = {};                         //!< Stream identifier
  std::string_view account;                        //!< Account name
  uint64_t order_id = {};                          //!< Order identifier
  std::string_view exchange;                       //!< Exchange
  std::string_view symbol;                         //!< Symbol
  roq::Side side = {};                             //!< Side
  roq::PositionEffect position_effect = {};        //!< Position effect
  roq::MarginMode margin_mode = {};                //!< Margin mode
  roq::QuantityType quantity_type = {};            //!< Type of quantity (requires ecxhange support)
  std::chrono::nanoseconds create_time_utc = {};   //!< Created timestamp (UTC)
  std::chrono::nanoseconds update_time_utc = {};   //!< Updated timestamp (UTC)
  std::string_view external_account;               //!< External account name
  std::string_view external_order_id;              //!< External order identifier
  std::string_view client_order_id;                //!< Client order identifier
  std::span<roq::Fill const> fills;                //!< List of fills
  std::string_view routing_id;                     //!< Routing identifier
  roq::UpdateType update_type = {};                //!< Update type
  std::chrono::nanoseconds sending_time_utc = {};  //!< Exchange sending timestamp (UTC)
  std::string_view user;                           //!< User name (optional, only relevant for drop-copy)
  uint32_t strategy_id = {};                       //!< Strategy identifier (optional)
};

template <>
inline constexpr std::string_view get_name<TradeUpdate>() {
  using namespace std::literals;
  return "trade_update"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::TradeUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::TradeUpdate const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(account="{}", )"
        R"(order_id={}, )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(side={}, )"
        R"(position_effect={}, )"
        R"(margin_mode={}, )"
        R"(quantity_type={}, )"
        R"(create_time_utc={}, )"
        R"(update_time_utc={}, )"
        R"(external_account="{}", )"
        R"(external_order_id="{}", )"
        R"(client_order_id="{}", )"
        R"(fills=[{}], )"
        R"(routing_id="{}", )"
        R"(update_type={}, )"
        R"(sending_time_utc={}, )"
        R"(user="{}", )"
        R"(strategy_id={})"
        R"(}})"sv,
        value.stream_id,
        value.account,
        value.order_id,
        value.exchange,
        value.symbol,
        value.side,
        value.position_effect,
        value.margin_mode,
        value.quantity_type,
        value.create_time_utc,
        value.update_time_utc,
        value.external_account,
        value.external_order_id,
        value.client_order_id,
        fmt::join(value.fills, ", "sv),
        value.routing_id,
        value.update_type,
        value.sending_time_utc,
        value.user,
        value.strategy_id);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::TradeUpdate>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::TradeUpdate> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(trade_update={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::TradeUpdate>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::TradeUpdate> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(trade_update={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
