/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/compat.h"
#include "roq/fixed_string.h"
#include "roq/span.h"

#include "roq/event.h"
#include "roq/message_info.h"

#include "roq/execution_instruction.h"
#include "roq/order_type.h"
#include "roq/position_effect.h"
#include "roq/side.h"
#include "roq/time_in_force.h"

namespace roq {

//! Fields required to create an order
struct ROQ_PUBLIC CreateOrder final {
  CreateOrder() = default;
  CreateOrder(CreateOrder &&) = default;
  CreateOrder(const CreateOrder &) = delete;

  std::string_view account;     //!< Account name (as known to the gateway)
  uint32_t order_id = 0;        //!< Order identifier (as known to client)
  std::string_view exchange;    //!< Exchange name
  std::string_view symbol;      //!< Symbol
  Side side = Side::UNDEFINED;  //!< Order side
  double quantity = std::numeric_limits<double>::quiet_NaN();  //!< Order quantity
  OrderType order_type = OrderType::UNDEFINED;                 //!< Order type
  double price = std::numeric_limits<double>::quiet_NaN();  //!< Limit price (depends on order_type)
  TimeInForce time_in_force = TimeInForce::UNDEFINED;       //!< Time in force
  PositionEffect position_effect = PositionEffect::UNDEFINED;  //!< Position effect
  ExecutionInstruction execution_instruction =
      ExecutionInstruction::UNDEFINED;                           //!< Execution instruction
  double stop_price = std::numeric_limits<double>::quiet_NaN();  //!< Stop price (depends on
                                                                 //!< order_type and time_in_force)
  double max_show_quantity =
      std::numeric_limits<double>::quiet_NaN();  //!< Order quantity visible to market (depends on
                                                 //!< exchange support)
  std::string_view order_template;               //!< Order template (as known to the gateway)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::CreateOrder> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::CreateOrder &value, Context &context) {
    return format_to(
        context.out(),

        R"({{)"
        R"(account="{}", )"
        R"(order_id={}, )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(side={}, )"
        R"(quantity={}, )"
        R"(order_type={}, )"
        R"(price={}, )"
        R"(time_in_force={}, )"
        R"(position_effect={}, )"
        R"(execution_instruction={}, )"
        R"(stop_price={}, )"
        R"(max_show_quantity={}, )"
        R"(order_template="{}")"

        R"(}})",
        value.account,
        value.order_id,
        value.exchange,
        value.symbol,
        value.side,
        value.quantity,
        value.order_type,
        value.price,
        value.time_in_force,
        value.position_effect,
        value.execution_instruction,
        value.stop_price,
        value.max_show_quantity,
        value.order_template);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::CreateOrder> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Event<roq::CreateOrder> &event, Context &context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(create_order={})"
        R"(}})",
        event.message_info,
        event.value);
  }
};
