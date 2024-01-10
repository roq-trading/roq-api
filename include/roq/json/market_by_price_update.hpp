/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include <ranges>

#include "roq/market_by_price_update.hpp"

#include "roq/cache/market_by_price.hpp"

#include "roq/json/mbp_update.hpp"

#include "roq/json/context.hpp"
#include "roq/json/datetime.hpp"
#include "roq/json/string.hpp"

namespace roq {
namespace json {

struct MarketByPriceUpdate final {
  MarketByPriceUpdate(Context const &context, roq::MarketByPriceUpdate const &value)
      : context_{context}, value_{value} {}
  MarketByPriceUpdate(
      Context const &context,
      roq::MarketByPriceUpdate const &value,
      std::span<roq::MBPUpdate const> const &bids,
      std::span<roq::MBPUpdate const> const &asks)
      : context_{context}, value_{value}, bids_{bids}, asks_{asks}, cache_{true} {}

  auto format_to(auto &context) const {
    using namespace std::literals;
    auto bids = cache_ ? bids_ : value_.bids;
    auto asks = cache_ ? asks_ : value_.asks;
    auto update_type = cache_ ? UpdateType::SNAPSHOT : value_.update_type;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"("stream_id":{},)"
        R"("exchange":{},)"
        R"("symbol":{},)"
        R"("bids":[{}],)"
        R"("asks":[{}],)"
        R"("update_type":{},)"
        R"("exchange_time_utc":{},)"
        R"("exchange_sequence":{})"
        R"(}})"sv,
        value_.stream_id,
        String{value_.exchange},
        String{value_.symbol},
        fmt::join(
            std::ranges::views::transform(
                bids,
                [this](auto &v) {
                  return MBPUpdate{context_, v};
                }),
            ","sv),
        fmt::join(
            std::ranges::views::transform(
                asks,
                [this](auto &v) {
                  return MBPUpdate{context_, v};
                }),
            ","sv),
        String{update_type},
        DateTime{value_.exchange_time_utc},
        value_.exchange_sequence);
    // note! remaining fields are internal
  }

 private:
  Context const &context_;
  roq::MarketByPriceUpdate const &value_;
  std::span<roq::MBPUpdate const> const bids_, asks_;
  bool const cache_ = {};
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::MarketByPriceUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::json::MarketByPriceUpdate const &value, format_context &context) const {
    return value.format_to(context);
  }
};
