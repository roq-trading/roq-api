/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include "roq/numbers.hpp"
#include "roq/side.hpp"
#include "roq/string_types.hpp"

namespace roq {

//! Represents a single trade (a match) as part of trade reporting by the exchange
struct ROQ_PUBLIC Trade final {
  Side side = {};                  //!< Side (by convention: side of the taker)
  double price = NaN;              //!< Price
  double quantity = NaN;           //!< Quantity
  ExternalTradeId trade_id;        //!< Trade identifier
  ExternalOrderId taker_order_id;  //!< External order identifier (taker)
  ExternalOrderId maker_order_id;  //!< External order identifier (maker)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Trade> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Trade const &value, Context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(side={}, )"
        R"(price={}, )"
        R"(quantity={}, )"
        R"(trade_id="{}", )"
        R"(taker_order_id="{}", )"
        R"(maker_order_id="{}")"
        R"(}})"sv,
        value.side,
        value.price,
        value.quantity,
        value.trade_id,
        value.taker_order_id,
        value.maker_order_id);
  }
};
