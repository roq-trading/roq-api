/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

namespace roq {
namespace oms {

struct ROQ_PUBLIC OrderUpdate final {
  std::string_view account;
  std::string_view exchange;
  std::string_view symbol;
  Side side = {};
  PositionEffect position_effect = {};
  double max_show_quantity = NaN;
  OrderType order_type = {};
  TimeInForce time_in_force = {};
  Mask<ExecutionInstruction> execution_instructions;
  std::chrono::nanoseconds create_time_utc = {};
  std::chrono::nanoseconds update_time_utc = {};
  std::string_view external_account;
  std::string_view external_order_id;
  std::string_view client_order_id;
  OrderStatus status = {};
  double quantity = NaN;
  double price = NaN;
  double stop_price = NaN;
  double remaining_quantity = NaN;
  double traded_quantity = NaN;
  double average_traded_price = NaN;
  double last_traded_quantity = NaN;
  double last_traded_price = NaN;
  Liquidity last_liquidity = {};
  std::string_view routing_id;
  uint32_t max_request_version = {};
  uint32_t max_response_version = {};
  uint32_t max_accepted_version = {};
  UpdateType update_type = {};
  std::chrono::nanoseconds sending_time_utc = {};
};

}  // namespace oms
}  // namespace roq

template <>
struct fmt::formatter<roq::oms::OrderUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::oms::OrderUpdate const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(account="{}", )"
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
        R"(client_order_id="{}", )"
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
        R"(sending_time_utc={})"
        R"(}})"sv,
        value.account,
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
        value.client_order_id,
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
        value.sending_time_utc);
  }
};
