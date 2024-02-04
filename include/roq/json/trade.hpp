/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/trade.hpp"

#include "roq/json/context.hpp"
#include "roq/json/decimal.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct Trade final {
  Trade(Context const &context, roq::Trade const &value) : context_{context}, value_{value} {}

  auto format_to(auto &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("side":{},)"
        R"("price":{},)"
        R"("quantity":{},)"
        R"("trade_id":{})"
        R"(}})"sv,
        String{value_.side},
        Decimal{value_.price, context_.price_decimals},
        Decimal{value_.quantity, context_.quantity_decimals},
        String{value_.trade_id});
  }

 private:
  Context const &context_;
  roq::Trade const &value_;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::Trade> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::json::Trade const &value, format_context &context) const { return value.format_to(context); }
};
