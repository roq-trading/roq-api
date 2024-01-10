/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include "roq/market_status.hpp"

#include "roq/json/datetime.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct MarketStatus final {
  explicit MarketStatus(roq::MarketStatus const &value) : value_{value} {}

  auto format_to(auto &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("stream_id":{},)"
        R"("exchange":{},)"
        R"("symbol":{},)"
        R"("trading_status":{},)"
        R"("exchange_time_utc":{})"
        R"(}})"sv,
        value_.stream_id,
        String{value_.exchange},
        String{value_.symbol},
        String{value_.trading_status},
        DateTime{value_.exchange_time_utc});
  }

 private:
  roq::MarketStatus const &value_;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::MarketStatus> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::json::MarketStatus const &value, format_context &context) const { return value.format_to(context); }
};
