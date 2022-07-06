/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/trade.hpp"

#include "roq/json/number.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct Trade final {
  explicit Trade(roq::Trade const &value) : value_(value) {}

  template <typename Context>
  auto format_to(Context &context) const {
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
        Number{value_.price},
        Number{value_.quantity},
        String{value_.trade_id});
  }

 private:
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
