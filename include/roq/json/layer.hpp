/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/layer.hpp"

#include "roq/json/number.hpp"

namespace roq {
namespace json {

struct Layer final {
  explicit Layer(roq::Layer const &value) : value_{value} {}

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
        Number{value_.bid_price},
        Number{value_.bid_quantity},
        Number{value_.ask_price},
        Number{value_.ask_quantity});
  }

 private:
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
