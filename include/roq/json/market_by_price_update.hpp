/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <fmt/compile.h>
#include <fmt/format.h>

#include <range/v3/view.hpp>

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
      std::span<const roq::MBPUpdate> const &bids,
      std::span<const roq::MBPUpdate> const &asks)
      : context_{context}, value_{value}, bids_{bids}, asks_{asks}, cache_{true} {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
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
        R"(}})"_cf,
        value_.stream_id,
        String{value_.exchange},
        String{value_.symbol},
        fmt::join(
            ranges::views::transform(
                bids,
                [this](auto const &v) {
                  return MBPUpdate{context_, v};
                }),
            ","sv),
        fmt::join(
            ranges::views::transform(
                asks,
                [this](auto const &v) {
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
  std::span<const roq::MBPUpdate> const bids_, asks_;
  bool const cache_ = {};
};

}  // namespace json
}  // namespace roq

template <>
struct fmt::formatter<roq::json::MarketByPriceUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::json::MarketByPriceUpdate const &value, Context &context) const {
    return value.format_to(context);
  }
};
