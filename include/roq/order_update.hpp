/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/event.hpp"
#include "roq/execution_instruction.hpp"
#include "roq/liquidity.hpp"
#include "roq/margin_mode.hpp"
#include "roq/mask.hpp"
#include "roq/name.hpp"
#include "roq/order_status.hpp"
#include "roq/order_type.hpp"
#include "roq/position_effect.hpp"
#include "roq/side.hpp"
#include "roq/time_in_force.hpp"
#include "roq/trace.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Update relating to current status of an order
struct ROQ_PUBLIC OrderUpdate final {
  uint16_t stream_id = {};                                                 //!< Stream identifier
  std::string_view account;                                                //!< Account name
  uint64_t order_id = {};                                                  //!< Order identifier
  std::string_view exchange;                                               //!< Exchange
  std::string_view symbol;                                                 //!< Symbol
  roq::Side side = {};                                                     //!< Side
  roq::PositionEffect position_effect = {};                                //!< Position effect
  roq::MarginMode margin_mode = {};                                        //!< Margin mode
  double max_show_quantity = std::numeric_limits<double>::quiet_NaN();     //!< Quantity visible to market (base currency, requires exchange support)
  roq::OrderType order_type = {};                                          //!< Order type
  roq::TimeInForce time_in_force = {};                                     //!< Time in force
  roq::Mask<roq::ExecutionInstruction> execution_instructions;             //!< Execution instructions
  std::chrono::nanoseconds create_time_utc = {};                           //!< Created timestamp (UTC)
  std::chrono::nanoseconds update_time_utc = {};                           //!< Updated timestamp (UTC)
  std::string_view external_account;                                       //!< External account name
  std::string_view external_order_id;                                      //!< External order identifier
  std::string_view client_order_id;                                        //!< Client order identifier
  roq::OrderStatus order_status = {};                                      //!< Order status
  double quantity = std::numeric_limits<double>::quiet_NaN();              //!< Quantity (base currency, total, indicative)
  double price = std::numeric_limits<double>::quiet_NaN();                 //!< Price
  double stop_price = std::numeric_limits<double>::quiet_NaN();            //!< Stop price (depends on order_type and time_in_force)
  double risk_exposure = std::numeric_limits<double>::quiet_NaN();         //!< Risk exposure
  double risk_exposure_change = std::numeric_limits<double>::quiet_NaN();  //!< Risk exposure change
  double remaining_quantity = std::numeric_limits<double>::quiet_NaN();    //!< Quantity (base currency, remaining)
  double traded_quantity = std::numeric_limits<double>::quiet_NaN();       //!< Quantity (base currency, total traded)
  double average_traded_price = std::numeric_limits<double>::quiet_NaN();  //!< Average price (total traded)
  double last_traded_quantity = std::numeric_limits<double>::quiet_NaN();  //!< Traded quantity (base currency, last trade)
  double last_traded_price = std::numeric_limits<double>::quiet_NaN();     //!< Traded price (last trade)
  roq::Liquidity last_liquidity = {};                                      //!< Liquidity indicator (last trade)
  std::string_view routing_id;                                             //!< Routing identifier
  uint32_t max_request_version = {};                                       //!< Last request version
  uint32_t max_response_version = {};                                      //!< Last response version
  uint32_t max_accepted_version = {};                                      //!< Last accepted version
  roq::UpdateType update_type = {};                                        //!< Update type
  std::chrono::nanoseconds sending_time_utc = {};                          //!< Exchange sending timestamp (UTC)
  std::string_view user;                                                   //!< User name (optional, only relevant for drop-copy)
  uint32_t strategy_id = {};                                               //!< Strategy identifier (optional)
};

template <>
inline constexpr std::string_view get_name<OrderUpdate>() {
  using namespace std::literals;
  return "order_update"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::OrderUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::OrderUpdate const &value, format_context &context) const {
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
        R"(max_show_quantity={}, )"
        R"(order_type={}, )"
        R"(time_in_force={}, )"
        R"(execution_instructions={}, )"
        R"(create_time_utc={}, )"
        R"(update_time_utc={}, )"
        R"(external_account="{}", )"
        R"(external_order_id="{}", )"
        R"(client_order_id="{}", )"
        R"(order_status={}, )"
        R"(quantity={}, )"
        R"(price={}, )"
        R"(stop_price={}, )"
        R"(risk_exposure={}, )"
        R"(risk_exposure_change={}, )"
        R"(remaining_quantity={}, )"
        R"(traded_quantity={}, )"
        R"(average_traded_price={}, )"
        R"(last_traded_quantity={}, )"
        R"(last_traded_price={}, )"
        R"(last_liquidity={}, )"
        R"(routing_id="{}", )"
        R"(max_request_version={}, )"
        R"(max_response_version={}, )"
        R"(max_accepted_version={}, )"
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
        value.max_show_quantity,
        value.order_type,
        value.time_in_force,
        value.execution_instructions,
        value.create_time_utc,
        value.update_time_utc,
        value.external_account,
        value.external_order_id,
        value.client_order_id,
        value.order_status,
        value.quantity,
        value.price,
        value.stop_price,
        value.risk_exposure,
        value.risk_exposure_change,
        value.remaining_quantity,
        value.traded_quantity,
        value.average_traded_price,
        value.last_traded_quantity,
        value.last_traded_price,
        value.last_liquidity,
        value.routing_id,
        value.max_request_version,
        value.max_response_version,
        value.max_accepted_version,
        value.update_type,
        value.sending_time_utc,
        value.user,
        value.strategy_id);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::OrderUpdate>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::OrderUpdate> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(order_update={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::OrderUpdate>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::OrderUpdate> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(order_update={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
