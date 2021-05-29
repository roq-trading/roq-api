/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/string_buffer.h"

#include "roq/execution_instruction.h"
#include "roq/liquidity.h"
#include "roq/order_status.h"
#include "roq/order_type.h"
#include "roq/position_effect.h"
#include "roq/side.h"
#include "roq/time_in_force.h"

namespace roq {

//! Update relating to current status of an order
struct ROQ_PUBLIC OrderUpdate final {
  uint16_t stream_id = {};                          //!< Stream identifier
  std::string_view account;                         //!< Account name (as known to the gateway)
  uint32_t order_id = {};                           //!< Order identifier (as known to client)
  std::string_view exchange;                        //!< Exchange name
  std::string_view symbol;                          //!< Symbol
  Side side = {};                                   //!< Side
  PositionEffect position_effect = {};              //!< Position effect
  double quantity = NaN;                            //!< Original quantity
  double max_show_quantity = NaN;                   //!< Quantity visible to market (requires exchange support)
  OrderType order_type = {};                        //!< Order type
  TimeInForce time_in_force = {};                   //!< Time in force
  ExecutionInstruction execution_instruction = {};  //!< Execution instruction
  std::string_view order_template;                  //!< Order template (as known to the gateway)
  std::chrono::nanoseconds create_time_utc = {};    //!< Created timestamp (UTC)
  std::chrono::nanoseconds update_time_utc = {};    //!< Updated timestamp (UTC)
  std::string_view external_account;                //!< External account name (as known to broker or exchange)
  std::string_view external_order_id;               //!< External order identifier (as known to broker or exchange)
  std::string_view routing_id;                      //!< Routing identifier
  OrderStatus status = {};                          //!< Order status
  double price = NaN;                               //!< Price
  double stop_price = NaN;                          //!< Stop price (depends on order_type and time_in_force)
  double remaining_quantity = NaN;                  //!< Remaining quantity
  double traded_quantity = NaN;                     //!< Traded quantity
  double average_traded_price = NaN;                //!< Average price (for traded quantity)
  double last_traded_price = NaN;                   //!< Last traded price (for last quantity)
  double last_traded_quantity = NaN;                //!< Last traded quantity
  Liquidity last_liquidity = {};                    //!< Liquidity indicator (for last trade)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::OrderUpdate> : public roq::formatter {
  template <typename Context>
  auto format(const roq::OrderUpdate &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(account="{}", )"
        R"(order_id={}, )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(side={}, )"
        R"(position_effect={}, )"
        R"(quantity={}, )"
        R"(max_show_quantity={}, )"
        R"(order_type={}, )"
        R"(time_in_force={}, )"
        R"(execution_instruction={}, )"
        R"(order_template="{}", )"
        R"(create_time_utc={}, )"
        R"(update_time_utc={}, )"
        R"(external_account="{}", )"
        R"(external_order_id="{}", )"
        R"(routing_id="{}", )"
        R"(status={}, )"
        R"(price={}, )"
        R"(stop_price={}, )"
        R"(remaining_quantity={}, )"
        R"(traded_quantity={}, )"
        R"(average_traded_price={}, )"
        R"(last_traded_price={}, )"
        R"(last_traded_quantity={}, )"
        R"(last_liquidity={})"
        R"(}})"_fmt,
        value.stream_id,
        value.account,
        value.order_id,
        value.exchange,
        value.symbol,
        value.side,
        value.position_effect,
        value.quantity,
        value.max_show_quantity,
        value.order_type,
        value.time_in_force,
        value.execution_instruction,
        value.order_template,
        value.create_time_utc,
        value.update_time_utc,
        value.external_account,
        value.external_order_id,
        value.routing_id,
        value.status,
        value.price,
        value.stop_price,
        value.remaining_quantity,
        value.traded_quantity,
        value.average_traded_price,
        value.last_traded_price,
        value.last_traded_quantity,
        value.last_liquidity);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::OrderUpdate> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::OrderUpdate> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(order_update={})"
        R"(}})"_fmt,
        event.message_info,
        event.value);
  }
};
