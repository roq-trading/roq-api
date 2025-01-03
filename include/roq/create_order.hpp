/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/core.h>

#include <magic_enum/magic_enum_format.hpp>

#include <string_view>

#include "roq/event.hpp"
#include "roq/execution_instruction.hpp"
#include "roq/limits.hpp"
#include "roq/margin_mode.hpp"
#include "roq/mask.hpp"
#include "roq/name.hpp"
#include "roq/order_type.hpp"
#include "roq/position_effect.hpp"
#include "roq/quantity_type.hpp"
#include "roq/side.hpp"
#include "roq/time_in_force.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Fields required to create an order
struct ROQ_PUBLIC CreateOrder final {
  std::string_view account;                                     //!< Account name
  uint64_t order_id = {};                                       //!< Order identifier
  std::string_view exchange;                                    //!< Exchange
  std::string_view symbol;                                      //!< Symbol
  roq::Side side = {};                                          //!< Order side
  roq::PositionEffect position_effect = {};                     //!< Position effect
  roq::MarginMode margin_mode = {};                             //!< Margin mode
  roq::QuantityType quantity_type = {};                         //!< Type of quantity (requires ecxhange support)
  double max_show_quantity = roq::NaN;                          //!< Quantity visible to market (requires exchange support)
  roq::OrderType order_type = {};                               //!< Order type
  roq::TimeInForce time_in_force = {};                          //!< Time in force
  roq::Mask<roq::ExecutionInstruction> execution_instructions;  //!< Execution instructions
  std::string_view request_template;                            //!< Request template (gateway configured)
  double quantity = roq::NaN;                                   //!< Order quantity
  double price = roq::NaN;                                      //!< Limit price (depends on order_type)
  double stop_price = roq::NaN;                                 //!< Stop price (depends on order_type and time_in_force)
  std::string_view routing_id;                                  //!< Routing identifier
  uint32_t strategy_id = {};                                    //!< Strategy identifier (optional)
};

template <>
inline constexpr std::string_view get_name<CreateOrder>() {
  using namespace std::literals;
  return "create_order"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::CreateOrder> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::CreateOrder const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(account="{}", )"
        R"(order_id={}, )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(side={}, )"
        R"(position_effect={}, )"
        R"(margin_mode={}, )"
        R"(quantity_type={}, )"
        R"(max_show_quantity={}, )"
        R"(order_type={}, )"
        R"(time_in_force={}, )"
        R"(execution_instructions={}, )"
        R"(request_template="{}", )"
        R"(quantity={}, )"
        R"(price={}, )"
        R"(stop_price={}, )"
        R"(routing_id="{}", )"
        R"(strategy_id={})"
        R"(}})"sv,
        value.account,
        value.order_id,
        value.exchange,
        value.symbol,
        value.side,
        value.position_effect,
        value.margin_mode,
        value.quantity_type,
        value.max_show_quantity,
        value.order_type,
        value.time_in_force,
        value.execution_instructions,
        value.request_template,
        value.quantity,
        value.price,
        value.stop_price,
        value.routing_id,
        value.strategy_id);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::CreateOrder>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::CreateOrder> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(create_order={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::CreateOrder>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::CreateOrder> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(create_order={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
