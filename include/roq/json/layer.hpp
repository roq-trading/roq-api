/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/layer.hpp"

#include "roq/json/context.hpp"
#include "roq/json/number.hpp"

namespace roq {
namespace json {

struct Layer final {
  explicit Layer(Context const &context, roq::Layer const &value) : context_{context}, value_{value} {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("bid_price":{},)"
        R"("bid_quantity":{},)"
        R"("ask_price":{},)"
        R"("ask_quantity":{})"
        R"(}})"sv,
        Number{value_.bid_price, context_.price_decimals},
        Number{value_.bid_quantity, context_.quantity_decimals},
        Number{value_.ask_price, context_.price_decimals},
        Number{value_.ask_quantity, context_.quantity_decimals});
  }

 private:
  Context const &context_;
  roq::Layer const &value_;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::Layer> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::json::Layer const &value, Context &context) const {
    return value.format_to(context);
  }
};
