/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/string_buffer.h"
#include "roq/uuid.h"

namespace roq {

//! Represents aggregate order book bid/ask information at a given depth
struct ROQ_PUBLIC Layer final {
  double bid_price = NaN;    //!< Bid price level
  double bid_quantity = {};  //!< Total quantity available at bid
  double ask_price = NaN;    //!< Ask price level
  double ask_quantity = {};  //!< Total quantity available at ask
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Layer> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::Layer &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(bid_price={}, )"
        R"(bid_quantity={}, )"
        R"(ask_price={}, )"
        R"(ask_quantity={})"
        R"(}})"sv,
        value.bid_price,
        value.bid_quantity,
        value.ask_price,
        value.ask_quantity);
  }
};
