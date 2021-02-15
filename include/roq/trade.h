/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/chrono.h"
#include "roq/fixed_string.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/span.h"
#include "roq/uuid.h"

#include "roq/side.h"

namespace roq {

//! Represents a single trade (match) as part of trade reporting by the exchange
struct ROQ_PUBLIC Trade final {
  Side side = Side::UNDEFINED;  //!< Side (by convention: side of the taker)
  double price = std::numeric_limits<double>::quiet_NaN();     //!< Price
  double quantity = std::numeric_limits<double>::quiet_NaN();  //!< Quantity
  roq::fixed_string<40> trade_id;  //!< Trade identifier (as known to client)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Trade> : public roq::formatter {
  template <typename Context>
  auto format(const roq::Trade &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(side={}, )"
        R"(price={}, )"
        R"(quantity={}, )"
        R"(trade_id="{}")"
        R"(}})"_fmt,
        value.side,
        value.price,
        value.quantity,
        value.trade_id);
  }
};
