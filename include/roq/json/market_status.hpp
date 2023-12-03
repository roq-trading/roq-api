/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include "roq/market_status.hpp"

#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct MarketStatus final {
  explicit MarketStatus(roq::MarketStatus const &value) : value_{value} {}

  auto format_to(auto &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("stream_id":{},)"
        R"("exchange":{},)"
        R"("symbol":{},)"
        R"("trading_status":{})"
        R"(}})"_cf,
        value_.stream_id,
        String{value_.exchange},
        String{value_.symbol},
        String{value_.trading_status});
  }

 private:
  roq::MarketStatus const &value_;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::MarketStatus> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::json::MarketStatus const &value, Context &context) const {
    return value.format_to(context);
  }
};
