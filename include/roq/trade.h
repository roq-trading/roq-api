/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/literals.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/string_buffer.h"
#include "roq/uuid.h"

#include "roq/side.h"

namespace roq {

//! Represents a single trade (match) as part of trade reporting by the exchange
struct ROQ_PUBLIC Trade final {
  Side side = {};                   //!< Side (by convention: side of the taker)
  double price = NaN;               //!< Price
  double quantity = NaN;            //!< Quantity
  roq::string_buffer<40> trade_id;  //!< Trade identifier
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Trade> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Trade &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(side={}, )"
        R"(price={}, )"
        R"(quantity={}, )"
        R"(trade_id="{}")"
        R"(}})"_sv,
        value.side,
        value.price,
        value.quantity,
        value.trade_id);
  }
};
