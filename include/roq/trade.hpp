/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/numbers.hpp"
#include "roq/string.hpp"
#include "roq/uuid.hpp"

#include "roq/side.hpp"

namespace roq {

//! Represents a single trade (match) as part of trade reporting by the exchange
struct ROQ_PUBLIC Trade final {
  Side side = {};            //!< Side (by convention: side of the taker)
  double price = NaN;        //!< Price
  double quantity = NaN;     //!< Quantity
  ExternalTradeId trade_id;  //!< Trade identifier
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Trade> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::Trade &value, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(side={}, )"
        R"(price={}, )"
        R"(quantity={}, )"
        R"(trade_id="{}")"
        R"(}})"sv,
        value.side,
        value.price,
        value.quantity,
        value.trade_id);
  }
};
