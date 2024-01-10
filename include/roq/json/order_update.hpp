/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include "roq/order_update.hpp"

#include "roq/json/context.hpp"
#include "roq/json/date.hpp"
#include "roq/json/datetime.hpp"
#include "roq/json/number.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct OrderUpdate final {
  OrderUpdate(Context const &context, roq::OrderUpdate const &value) : context_{context}, value_{value} {}

  auto format_to(auto &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("stream_id":{},)"
        R"("account":{},)"
        R"("order_id":{},)"
        R"("exchange":{},)"
        R"("symbol":{},)"
        R"("side":{},)"
        R"("position_effect":{},)"
        R"("max_show_quantity":{},)"
        R"("order_type":{},)"
        R"("time_in_force":{},)"
        R"("execution_instructions":{},)"
        R"("create_time_utc":{},)"
        R"("update_time_utc":{},)"
        R"("external_account":{},)"
        R"("external_order_id":{},)"
        R"("status":{},)"
        R"("quantity":{},)"
        R"("price":{},)"
        R"("stop_price":{},)"
        R"("remaining_quantity":{},)"
        R"("traded_quantity":{},)"
        R"("average_traded_price":{},)"
        R"("last_traded_quantity":{},)"
        R"("last_traded_price":{},)"
        R"("last_liquidity":{},)"
        R"("routing_id":{},)"
        R"("max_request_version":{},)"
        R"("max_response_version":{},)"
        R"("max_accepted_version":{},)"
        R"("update_type":{})"
        R"(}})"sv,
        value_.stream_id,
        String{value_.account},
        value_.order_id,
        String{value_.exchange},
        String{value_.symbol},
        String{value_.side},
        String{value_.position_effect},
        Number{value_.max_show_quantity, context_.quantity_decimals},
        String{value_.order_type},
        String{value_.time_in_force},
        String{value_.execution_instructions},
        DateTime{value_.create_time_utc},
        DateTime{value_.update_time_utc},
        String{value_.external_account},
        String{value_.external_order_id},
        String{value_.status},
        Number{value_.quantity, context_.quantity_decimals},
        Number{value_.price, context_.price_decimals},
        Number{value_.stop_price, context_.price_decimals},
        Number{value_.remaining_quantity, context_.quantity_decimals},
        Number{value_.traded_quantity, context_.quantity_decimals},
        Number{value_.average_traded_price, context_.price_decimals},  // XXX rounded ???
        Number{value_.last_traded_quantity, context_.quantity_decimals},
        Number{value_.last_traded_price, context_.price_decimals},
        String{value_.last_liquidity},
        String{value_.routing_id},
        value_.max_request_version,
        value_.max_response_version,
        value_.max_accepted_version,
        String{value_.update_type},
        String{value_.user});
  }

 private:
  Context const &context_;
  roq::OrderUpdate const &value_;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::OrderUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::json::OrderUpdate const &value, format_context &context) const { return value.format_to(context); }
};
