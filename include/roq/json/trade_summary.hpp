/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <ranges>

#include "roq/trade_summary.hpp"

#include "roq/json/trade.hpp"

#include "roq/json/context.hpp"
#include "roq/json/datetime.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct TradeSummary final {
  TradeSummary(Context const &context, roq::TradeSummary const &value) : context_{context}, value_{value} {}

  auto format_to(auto &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("stream_id":{},)"
        R"("exchange":{},)"
        R"("symbol":{},)"
        R"("trades":[{}],)"
        R"("exchange_sequence":{},)"
        R"("exchange_sequence":{})"
        R"(}})"sv,
        value_.stream_id,
        String{value_.exchange},
        String{value_.symbol},
        fmt::join(
            std::ranges::views::transform(
                value_.trades,
                [this](auto &v) {
                  return Trade{context_, v};
                }),
            ","),
        DateTime{value_.exchange_time_utc},
        value_.exchange_sequence);
  }

 private:
  Context const &context_;
  roq::TradeSummary const &value_;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::TradeSummary> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::json::TradeSummary const &value, format_context &context) const { return value.format_to(context); }
};
