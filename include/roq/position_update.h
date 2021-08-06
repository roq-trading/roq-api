/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/string_buffer.h"

#include "roq/side.h"

namespace roq {

//! Update relating to current position for a symbol/side/account
struct ROQ_PUBLIC PositionUpdate final {
  uint16_t stream_id = {};               //!< Stream identifier
  std::string_view account;              //!< Account name
  std::string_view exchange;             //!< Exchange
  std::string_view symbol;               //!< Symbol
  Side side = {};                        //!< Side
  double position = NaN;                 //!< Current position
  uint32_t last_trade_id = {};           //!< Last processed trade identifier
  double position_cost = NaN;            //!< Position cost
  double position_yesterday = NaN;       //!< Position as of yesterday
  double position_cost_yesterday = NaN;  //!< Position cost as of yesterday
  std::string_view external_account;     //!< External account name
};

}  // namespace roq

template <>
struct fmt::formatter<roq::PositionUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::PositionUpdate &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(account="{}", )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(side={}, )"
        R"(position={}, )"
        R"(last_trade_id={}, )"
        R"(position_cost={}, )"
        R"(position_yesterday={}, )"
        R"(position_cost_yesterday={}, )"
        R"(external_account="{}")"
        R"(}})"_sv,
        value.stream_id,
        value.account,
        value.exchange,
        value.symbol,
        value.side,
        value.position,
        value.last_trade_id,
        value.position_cost,
        value.position_yesterday,
        value.position_cost_yesterday,
        value.external_account);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::PositionUpdate> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Event<roq::PositionUpdate> &event, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(position_update={})"
        R"(}})"_sv,
        event.message_info,
        event.value);
  }
};
