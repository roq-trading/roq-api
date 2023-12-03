/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include "roq/trade.hpp"

#include "roq/json/context.hpp"
#include "roq/json/number.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct Trade final {
  Trade(Context const &context, roq::Trade const &value) : context_{context}, value_{value} {}

  auto format_to(auto &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("side":{},)"
        R"("price":{},)"
        R"("quantity":{},)"
        R"("trade_id":{})"
        R"(}})"_cf,
        String{value_.side},
        Number{value_.price, context_.price_decimals},
        Number{value_.quantity, context_.quantity_decimals},
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
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::json::Trade const &value, Context &context) const {
    return value.format_to(context);
  }
};
