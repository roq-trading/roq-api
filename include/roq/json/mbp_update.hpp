/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/mbp_update.hpp"

#include "roq/json/context.hpp"
#include "roq/json/decimal.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct MBPUpdate final {
  MBPUpdate(Context const &context, roq::MBPUpdate const &value) : context_{context}, value_{value} {}

  auto format_to(auto &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("price":{},)"
        R"("quantity":{},)"
        R"("implied_quantity":{},)"
        R"("number_of_orders":{},)"
        R"("update_action":{},)"
        R"("price_level":{})"
        R"(}})"sv,
        Decimal{value_.price, context_.price_decimals},
        Decimal{value_.quantity, context_.quantity_decimals},
        Decimal{value_.implied_quantity},
        value_.number_of_orders,
        String{value_.update_action},
        value_.price_level);
  }

 private:
  Context const &context_;
  roq::MBPUpdate const &value_;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::MBPUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::json::MBPUpdate const &value, format_context &context) const { return value.format_to(context); }
};
