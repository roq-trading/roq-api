/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/numbers.hpp"
#include "roq/span.hpp"
#include "roq/string_types.hpp"
#include "roq/uuid.hpp"

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
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Layer const &value, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
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
