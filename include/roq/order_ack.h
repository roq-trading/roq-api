/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/string_buffer.h"

#include "roq/error.h"
#include "roq/origin.h"
#include "roq/request_status.h"
#include "roq/request_type.h"
#include "roq/side.h"

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

}  // namespace roq

template <>
struct fmt::formatter<roq::OrderAck> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::OrderAck &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
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
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::Event<roq::OrderAck> &event, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(order_ack={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
