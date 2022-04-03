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
#include "roq/mask.hpp"
#include "roq/message_info.hpp"
#include "roq/numbers.hpp"
#include "roq/string.hpp"

#include "roq/fill.hpp"
#include "roq/position_effect.hpp"
#include "roq/side.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Update relating to order being partially or fully filled
struct ROQ_PUBLIC TradeUpdate final {
  uint16_t stream_id = {};                        //!< Stream identifier
  std::string_view account;                       //!< Account name
  uint32_t order_id = {};                         //!< Order identifier
  std::string_view exchange;                      //!< Exchange
  std::string_view symbol;                        //!< Symbol
  Side side = {};                                 //!< Side
  PositionEffect position_effect = {};            //!< Position effect
  std::chrono::nanoseconds create_time_utc = {};  //!< Created timestamp (UTC)
  std::chrono::nanoseconds update_time_utc = {};  //!< Updated timestamp (UTC)
  std::string_view external_account;              //!< External account name
  std::string_view external_order_id;             //!< External order identifier
  std::span<Fill> fills;                          //!< List of fills
  std::string_view routing_id;                    //!< Routing identifier
  UpdateType update_type = {};                    //!< Update type
};

}  // namespace roq

template <>
struct fmt::formatter<roq::TradeUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::TradeUpdate &value, Context &context) {
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
        R"(create_time_utc={}, )"
        R"(update_time_utc={}, )"
        R"(external_account="{}", )"
        R"(external_order_id="{}", )"
        R"(fills=[{}], )"
        R"(routing_id="{}", )"
        R"(update_type={})"
        R"(}})"sv,
        value.stream_id,
        value.account,
        value.order_id,
        value.exchange,
        value.symbol,
        value.side,
        value.position_effect,
        value.create_time_utc,
        value.update_time_utc,
        value.external_account,
        value.external_order_id,
        fmt::join(value.fills, ", "sv),
        value.routing_id,
        value.update_type);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::TradeUpdate> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::Event<roq::TradeUpdate> &event, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(trade_update={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
