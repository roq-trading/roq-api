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
#include "roq/name.hpp"
#include "roq/numbers.hpp"
#include "roq/string_types.hpp"
#include "roq/trace.hpp"

#include "roq/error.hpp"
#include "roq/origin.hpp"
#include "roq/request_status.hpp"
#include "roq/request_type.hpp"
#include "roq/side.hpp"

namespace roq {

//! Acknowledgement that a create/modify/cancel order request has been seen by gateway/exchange
struct ROQ_PUBLIC OrderAck final {
  uint16_t stream_id = {};                           //!< Stream identifier
  std::string_view account;                          //!< Account name
  uint32_t order_id = {};                            //!< Order identifier
  std::string_view exchange;                         //!< Exchange
  std::string_view symbol;                           //!< Symbol
  Side side = {};                                    //!< Side
  RequestType type = {};                             //!< Request type
  Origin origin = {};                                //!< Origin of ack
  RequestStatus status = {};                         //!< Request status
  Error error = {};                                  //!< Error code
  std::string_view text;                             //!< Descriptive text
  std::string_view request_id;                       //!< Request identifier
  std::string_view external_account;                 //!< External account name
  std::string_view external_order_id;                //!< External order identifier
  std::string_view routing_id;                       //!< Routing identifier
  uint32_t version = {};                             //!< Version number (strictly increasing, optional)
  std::chrono::nanoseconds round_trip_latency = {};  //!< Round-trip latency between gateway and exchange
};

template <>
inline constexpr std::string_view get_name<OrderAck>() {
  using namespace std::literals;
  return "order_ack"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::OrderAck> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::OrderAck &value, Context &context) {
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
        R"(type={}, )"
        R"(origin={}, )"
        R"(status={}, )"
        R"(error={}, )"
        R"(text="{}", )"
        R"(request_id="{}", )"
        R"(external_account="{}", )"
        R"(external_order_id="{}", )"
        R"(routing_id="{}", )"
        R"(version={}, )"
        R"(round_trip_latency={})"
        R"(}})"sv,
        value.stream_id,
        value.account,
        value.order_id,
        value.exchange,
        value.symbol,
        value.side,
        value.type,
        value.origin,
        value.status,
        value.error,
        value.text,
        value.request_id,
        value.external_account,
        value.external_order_id,
        value.routing_id,
        value.version,
        value.round_trip_latency);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::OrderAck> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::Event<roq::OrderAck> &event, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(order_ack={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::OrderAck const> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::Trace<roq::OrderAck const> &event, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(trace_info={}, )"
        R"(order_ack={})"
        R"(}})"sv,
        event.trace_info,
        event.value);
  }
};
