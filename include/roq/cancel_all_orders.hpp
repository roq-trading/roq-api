/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/core.h>

#include <magic_enum/magic_enum_format.hpp>

#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/side.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Cancel all orders (best effort)
struct ROQ_PUBLIC CancelAllOrders final {
  std::string_view account;   //!< Account name
  uint64_t order_id = {};     //!< Order identifier
  std::string_view exchange;  //!< Exchange
  std::string_view symbol;    //!< Symbol
  uint32_t strategy_id = {};  //!< Strategy identifier (optional)
  roq::Side side = {};        //!< Side
};

template <>
inline constexpr std::string_view get_name<CancelAllOrders>() {
  using namespace std::literals;
  return "cancel_all_orders"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::CancelAllOrders> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::CancelAllOrders const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(account="{}", )"
        R"(order_id={}, )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(strategy_id={}, )"
        R"(side={})"
        R"(}})"sv,
        value.account,
        value.order_id,
        value.exchange,
        value.symbol,
        value.strategy_id,
        value.side);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::CancelAllOrders>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::CancelAllOrders> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(cancel_all_orders={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::CancelAllOrders>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::CancelAllOrders> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(cancel_all_orders={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
