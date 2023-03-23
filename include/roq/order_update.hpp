/* Copyright (c) 2017-2023, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/compile.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/event.hpp"
#include "roq/execution_instruction.hpp"
#include "roq/liquidity.hpp"
#include "roq/mask.hpp"
#include "roq/name.hpp"
#include "roq/numbers.hpp"
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
  uint16_t stream_id = {};                            //!< Stream identifier
  std::string_view account;                           //!< Account name
  uint32_t order_id = {};                             //!< Order identifier
  std::string_view exchange;                          //!< Exchange
  std::string_view symbol;                            //!< Symbol
  Side side = {};                                     //!< Side
  PositionEffect position_effect = {};                //!< Position effect
  double max_show_quantity = NaN;                     //!< Quantity visible to market (requires exchange support)
  OrderType order_type = {};                          //!< Order type
  TimeInForce time_in_force = {};                     //!< Time in force
  Mask<ExecutionInstruction> execution_instructions;  //!< Execution instructions
  std::chrono::nanoseconds create_time_utc = {};      //!< Created timestamp (UTC)
  std::chrono::nanoseconds update_time_utc = {};      //!< Updated timestamp (UTC)
  std::string_view external_account;                  //!< External account name
  std::string_view external_order_id;                 //!< External order identifier
  OrderStatus status = {};                            //!< Order status
  double quantity = NaN;                              //!< Quantity (total, indicative)
  double price = NaN;                                 //!< Price
  double stop_price = NaN;                            //!< Stop price (depends on order_type and time_in_force)
  double remaining_quantity = NaN;                    //!< Quantity (remaining)
  double traded_quantity = NaN;                       //!< Quantity (total traded)
  double average_traded_price = NaN;                  //!< Average price (total traded)
  double last_traded_quantity = NaN;                  //!< Traded quantity (last trade)
  double last_traded_price = NaN;                     //!< Traded price (last trade)
  Liquidity last_liquidity = {};                      //!< Liquidity indicator (last trade)
  std::string_view routing_id;                        //!< Routing identifier
  uint32_t max_request_version = {};                  //!< Last request version
  uint32_t max_response_version = {};                 //!< Last response version
  uint32_t max_accepted_version = {};                 //!< Last accepted version
  UpdateType update_type = {};                        //!< Update type
  std::string_view user;                              //!< User name (optional, only relevant for drop-copy)
};

template <>
inline constexpr std::string_view get_name<OrderUpdate>() {
  using namespace std::literals;
  return "order_update"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::OrderUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::OrderUpdate const &value, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
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
        R"(max_show_quantity={}, )"
        R"(order_type={}, )"
        R"(time_in_force={}, )"
        R"(execution_instructions={}, )"
        R"(create_time_utc={}, )"
        R"(update_time_utc={}, )"
        R"(external_account="{}", )"
        R"(external_order_id="{}", )"
        R"(status={}, )"
        R"(quantity={}, )"
        R"(price={}, )"
        R"(stop_price={}, )"
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
        R"(user="{}")"
        R"(}})"_cf,
        value.stream_id,
        value.account,
        value.order_id,
        value.exchange,
        value.symbol,
        value.side,
        value.position_effect,
        value.max_show_quantity,
        value.order_type,
        value.time_in_force,
        value.execution_instructions,
        value.create_time_utc,
        value.update_time_utc,
        value.external_account,
        value.external_order_id,
        value.status,
        value.quantity,
        value.price,
        value.stop_price,
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
        value.user);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::OrderUpdate>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Event<roq::OrderUpdate> const &event, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(order_update={}, )"
        R"(message_info={})"
        R"(}})"_cf,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::OrderUpdate>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Trace<roq::OrderUpdate> const &event, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(order_update={}, )"
        R"(trace_info={})"
        R"(}})"_cf,
        event.value,
        event.trace_info);
  }
};
