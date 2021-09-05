/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include "roq/api.h"

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
  ExecutionInstruction execution_instruction = {};
  std::string_view order_template;
  std::chrono::nanoseconds create_time_utc = {};
  std::chrono::nanoseconds update_time_utc = {};
  std::string_view external_account;
  std::string_view external_order_id;
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
};

}  // namespace oms
}  // namespace roq

template <>
struct fmt::formatter<roq::oms::OrderUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::oms::OrderUpdate &value, Context &context) {
    using namespace roq::literals;
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
        R"(last_traded_quantity={}, )"
        R"(last_traded_price={}, )"
        R"(last_liquidity={})"
        R"(}})"_sv,
        value.account,
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
        value.last_traded_quantity,
        value.last_traded_price,
        value.last_liquidity);
  }
};
