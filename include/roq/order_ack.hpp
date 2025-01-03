/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/core.h>

#include <magic_enum/magic_enum_format.hpp>

#include <chrono>
#include <string_view>

#include "roq/error.hpp"
#include "roq/event.hpp"
#include "roq/limits.hpp"
#include "roq/margin_mode.hpp"
#include "roq/name.hpp"
#include "roq/origin.hpp"
#include "roq/position_effect.hpp"
#include "roq/quantity_type.hpp"
#include "roq/request_status.hpp"
#include "roq/request_type.hpp"
#include "roq/side.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Acknowledgement that a create/modify/cancel order request has been seen by gateway/exchange
struct ROQ_PUBLIC OrderAck final {
  uint16_t stream_id = {};                           //!< Stream identifier
  std::string_view account;                          //!< Account name
  uint64_t order_id = {};                            //!< Order identifier
  std::string_view exchange;                         //!< Exchange
  std::string_view symbol;                           //!< Symbol
  roq::Side side = {};                               //!< Side
  roq::PositionEffect position_effect = {};          //!< Position effect
  roq::MarginMode margin_mode = {};                  //!< Margin mode
  roq::QuantityType quantity_type = {};              //!< Type of quantity (requires ecxhange support)
  roq::RequestType request_type = {};                //!< Request type
  roq::Origin origin = {};                           //!< Origin of ack
  roq::RequestStatus request_status = {};            //!< Request status
  roq::Error error = {};                             //!< Error code
  std::string_view text;                             //!< Descriptive text
  std::string_view request_id;                       //!< Request identifier
  std::string_view external_account;                 //!< External account name
  std::string_view external_order_id;                //!< External order identifier
  std::string_view client_order_id;                  //!< Client order identifier
  double quantity = roq::NaN;                        //!< Quantity (total, indicative)
  double price = roq::NaN;                           //!< Price
  double stop_price = roq::NaN;                      //!< Stop price (depends on order_type and time_in_force)
  std::string_view routing_id;                       //!< Routing identifier
  uint32_t version = {};                             //!< Version number (strictly increasing, optional)
  double risk_exposure = roq::NaN;                   //!< Risk exposure
  double risk_exposure_change = roq::NaN;            //!< Risk exposure change
  double traded_quantity = roq::NaN;                 //!< Quantity (total traded)
  std::chrono::nanoseconds round_trip_latency = {};  //!< Round-trip latency (interpretation depends on origin)
  std::string_view user;                             //!< User name (optional, only relevant for drop-copy)
  uint32_t strategy_id = {};                         //!< Strategy identifier (optional)
};

template <>
inline constexpr std::string_view get_name<OrderAck>() {
  using namespace std::literals;
  return "order_ack"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::OrderAck> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::OrderAck const &value, format_context &context) const {
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
        R"(request_type={}, )"
        R"(origin={}, )"
        R"(request_status={}, )"
        R"(error={}, )"
        R"(text="{}", )"
        R"(request_id="{}", )"
        R"(external_account="{}", )"
        R"(external_order_id="{}", )"
        R"(client_order_id="{}", )"
        R"(quantity={}, )"
        R"(price={}, )"
        R"(stop_price={}, )"
        R"(routing_id="{}", )"
        R"(version={}, )"
        R"(risk_exposure={}, )"
        R"(risk_exposure_change={}, )"
        R"(traded_quantity={}, )"
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
        value.position_effect,
        value.margin_mode,
        value.quantity_type,
        value.request_type,
        value.origin,
        value.request_status,
        value.error,
        value.text,
        value.request_id,
        value.external_account,
        value.external_order_id,
        value.client_order_id,
        value.quantity,
        value.price,
        value.stop_price,
        value.routing_id,
        value.version,
        value.risk_exposure,
        value.risk_exposure_change,
        value.traded_quantity,
        value.round_trip_latency,
        value.user,
        value.strategy_id);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::OrderAck>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::OrderAck> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(order_ack={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::OrderAck>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::OrderAck> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(order_ack={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
