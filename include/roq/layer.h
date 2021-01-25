/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/fixed_string.h"
#include "roq/span.h"
#include "roq/uuid.h"

namespace roq {

//! Represents aggregate order book bid/ask information at a given depth
struct ROQ_PUBLIC Layer final {
  double bid_price = std::numeric_limits<double>::quiet_NaN();  //!< Bid price level
  double bid_quantity = 0.0;                                    //!< Total quantity available at bid
  double ask_price = std::numeric_limits<double>::quiet_NaN();  //!< Ask price level
  double ask_quantity = 0.0;                                    //!< Total quantity available at ask
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Layer> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Layer &value, Context &context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(bid_price={}, )"
        R"(bid_quantity={}, )"
        R"(ask_price={}, )"
        R"(ask_quantity={})"
        R"(}})",
        value.bid_price,
        value.bid_quantity,
        value.ask_price,
        value.ask_quantity);
  }
};
