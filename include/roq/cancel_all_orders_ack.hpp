/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/error.hpp"
#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/origin.hpp"
#include "roq/request_status.hpp"
#include "roq/side.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Acknowledgement that a cancel all orders request has been seen by gateway/exchange
struct ROQ_PUBLIC CancelAllOrdersAck final {
  uint16_t stream_id = {};                           //!< Stream identifier
  std::string_view account;                          //!< Account name
  uint64_t order_id = {};                            //!< Order identifier
  std::string_view exchange;                         //!< Exchange
  std::string_view symbol;                           //!< Symbol
  Side side = {};                                    //!< Side
  Origin origin = {};                                //!< Origin of ack
  RequestStatus request_status = {};                 //!< Request status
  Error error = {};                                  //!< Error code
  std::string_view text;                             //!< Descriptive text
  std::string_view request_id;                       //!< Request identifier
  std::string_view external_account;                 //!< External account name
  uint32_t number_of_affected_orders = {};           //!< Number of affected orders (optional, indicative)
  std::chrono::nanoseconds round_trip_latency = {};  //!< Round-trip latency between gateway and exchange
  std::string_view user;                             //!< User name (optional, only relevant for drop-copy)
  uint32_t strategy_id = {};                         //!< Strategy identifier (optional)
};

template <>
inline constexpr std::string_view get_name<CancelAllOrdersAck>() {
  using namespace std::literals;
  return "cancel_all_orders_ack"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::CancelAllOrdersAck> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::CancelAllOrdersAck const &value, format_context &context) const {
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
        R"(origin={}, )"
        R"(request_status={}, )"
        R"(error={}, )"
        R"(text="{}", )"
        R"(request_id="{}", )"
        R"(external_account="{}", )"
        R"(number_of_affected_orders={}, )"
        R"(round_trip_latency={}, )"
        R"(user="{}", )"
        R"(strategy_id={})"
        R"(}})"sv,
        value.stream_id,
        value.account,
        value.order_id,
        value.exchange,
        value.symbol,
        value.side,
        value.origin,
        value.request_status,
        value.error,
        value.text,
        value.request_id,
        value.external_account,
        value.number_of_affected_orders,
        value.round_trip_latency,
        value.user,
        value.strategy_id);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::CancelAllOrdersAck>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::CancelAllOrdersAck> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(cancel_all_orders_ack={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::CancelAllOrdersAck>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::CancelAllOrdersAck> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(cancel_all_orders_ack={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
