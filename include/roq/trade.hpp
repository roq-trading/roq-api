/* Copyright (c) 2017-2026, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include <magic_enum/magic_enum_format.hpp>

#include "roq/limits.hpp"
#include "roq/side.hpp"
#include "roq/string_types.hpp"

namespace roq {

//! Represents a single trade (a match) as part of trade reporting by the exchange
struct ROQ_PUBLIC Trade final {
  roq::Side side = {};                  //!< Side (by convention: side of the taker)
  double price = roq::NaN;              //!< Price
  double quantity = roq::NaN;           //!< Quantity
  roq::ExternalTradeId trade_id;        //!< Trade identifier
  roq::ExternalOrderId taker_order_id;  //!< External order identifier (taker)
  roq::ExternalOrderId maker_order_id;  //!< External order identifier (maker)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Trade> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trade const &value, format_context &context) const {
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