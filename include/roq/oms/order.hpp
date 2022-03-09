/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

namespace roq {
namespace oms {

//! Order (state)
struct ROQ_PUBLIC Order final {
  uint8_t user_id = {};
  uint16_t stream_id = {};
  string_buffer<MAX_LENGTH_ACCOUNT> account;
  uint32_t order_id = {};
  string_buffer<MAX_LENGTH_EXCHANGE> exchange;
  string_buffer<MAX_LENGTH_SYMBOL> symbol;
  Side side = {};
  PositionEffect position_effect = {};
  double max_show_quantity = NaN;
  OrderType order_type = {};
  TimeInForce time_in_force = {};
  ExecutionInstruction execution_instruction = {};
  string_buffer<MAX_LENGTH_ORDER_TEMPLATE> order_template;
  std::chrono::nanoseconds create_time_utc = {};
  std::chrono::nanoseconds update_time_utc = {};
  string_buffer<MAX_LENGTH_EXTERNAL_ACCOUNT> external_account;
  string_buffer<MAX_LENGTH_EXTERNAL_ORDER_ID> external_order_id;
  OrderStatus status = {};
  double quantity = NaN;
  double price = NaN;
  double stop_price = NaN;
  double remaining_quantity = NaN;
  double traded_quantity = NaN;
  double average_traded_price = NaN;
  double last_traded_price = NaN;
  double last_traded_quantity = NaN;
  Liquidity last_liquidity = {};
  string_buffer<MAX_LENGTH_ROUTING_ID> routing_id;
  uint32_t max_request_version = {};
  uint32_t max_response_version = {};
  uint32_t max_accepted_version = {};
  Decimals price_decimals = {};
  Decimals quantity_decimals = {};
  UpdateType update_type = {};
};

}  // namespace oms
}  // namespace roq

template <>
struct fmt::formatter<roq::oms::Order> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename C>
  auto format(const roq::oms::Order &value, C &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(user_id={}, )"
        R"(account="{}", )"
        R"(order_id={}, )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(side={}, )"
        R"(position_effect={}, )"
        R"(max_show_quantity={}, )"
        R"(order_type={}, )"
        R"(time_in_force={}, )"
        R"(execution_instruction={}, )"
        R"(order_template="{}", )"
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
        R"(last_traded_price={}, )"
        R"(last_traded_quantity={}, )"
        R"(last_liquidity={}, )"
        R"(routing_id="{}", )"
        R"(max_request_version={}, )"
        R"(max_response_version={}, )"
        R"(max_accepted_version={}, )"
        R"(price_decimals={}, )"
        R"(quantity_decimals={}, )"
        R"(update_type={})"
        R"(}})"sv,
        value.user_id,
        value.account,
        value.order_id,
        value.exchange,
        value.symbol,
        value.side,
        value.position_effect,
        value.max_show_quantity,
        value.order_type,
        value.time_in_force,
        value.execution_instruction,
        value.order_template,
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
        value.last_traded_price,
        value.last_traded_quantity,
        value.last_liquidity,
        value.routing_id,
        value.max_request_version,
        value.max_response_version,
        value.max_accepted_version,
        value.price_decimals,
        value.quantity_decimals,
        value.update_type);
  }
};
