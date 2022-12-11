/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <range/v3/view.hpp>

#include "roq/trade_summary.hpp"

#include "roq/json/trade.hpp"

#include "roq/json/datetime.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct TradeSummary final {
  explicit TradeSummary(roq::TradeSummary const &value) : value_{value} {}

  template <typename Context>
  auto format_to(Context &context) const {
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
        fmt::join(ranges::views::transform(value_.trades, [](auto const &v) { return Trade{v}; }), ","),
        DateTime{value_.exchange_time_utc},
        value_.exchange_sequence);
  }

 private:
  roq::TradeSummary const &value_;
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::TradeSummary> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::json::TradeSummary const &value, Context &context) const {
    return value.format_to(context);
  }
};
