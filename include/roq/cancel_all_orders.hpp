/* Copyright (c) 2017-2023, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/compile.h>
#include <fmt/format.h>

#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/side.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Cancel all orders (best effort)
struct ROQ_PUBLIC CancelAllOrders final {
  std::string_view account;   //!< Account name
  std::string_view exchange;  //!< Exchange
  std::string_view symbol;    //!< Symbol
  Side side = {};             //!< Side
  uint32_t strategy_id = {};  //!< Strategy identifier (optional)
};

template <>
inline constexpr std::string_view get_name<CancelAllOrders>() {
  using namespace std::literals;
  return "cancel_all_orders"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::CancelAllOrders> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::CancelAllOrders const &value, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(account="{}", )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(side={}, )"
        R"(strategy_id={})"
        R"(}})"_cf,
        value.account,
        value.exchange,
        value.symbol,
        value.side,
        value.strategy_id);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::CancelAllOrders>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Event<roq::CancelAllOrders> const &event, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(cancel_all_orders={}, )"
        R"(message_info={})"
        R"(}})"_cf,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::CancelAllOrders>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Trace<roq::CancelAllOrders> const &event, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(cancel_all_orders={}, )"
        R"(trace_info={})"
        R"(}})"_cf,
        event.value,
        event.trace_info);
  }
};
