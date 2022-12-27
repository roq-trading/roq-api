/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include <range/v3/view.hpp>

#include "roq/trade_summary.hpp"

#include "roq/json/trade.hpp"

#include "roq/json/context.hpp"
#include "roq/json/datetime.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct TradeSummary final {
  TradeSummary(Context const &context, roq::TradeSummary const &value) : context_{context}, value_{value} {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("stream_id":{},)"
        R"("exchange":{},)"
        R"("symbol":{},)"
        R"("trades":[{}],)"
        R"("exchange_sequence":{},)"
        R"("exchange_sequence":{})"
        R"(}})"_cf,
        value_.stream_id,
        String{value_.exchange},
        String{value_.symbol},
        fmt::join(
            ranges::views::transform(
                value_.trades,
                [this](auto const &v) {
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
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::json::TradeSummary const &value, Context &context) const {
    return value.format_to(context);
  }
};
