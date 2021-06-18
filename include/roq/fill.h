/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/string_buffer.h"
#include "roq/uuid.h"

#include "roq/liquidity.h"

namespace roq {

//! Represents a single fill (match) when an order is being partially or fully filled
struct ROQ_PUBLIC Fill final {
  roq::string_buffer<40> external_trade_id;  //!< External trade identifier
  double quantity = NaN;                     //!< Quantity
  double price = NaN;                        //!< Price
  Liquidity liquidity = {};                  //!< Liquidity indicator
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Fill> : public roq::formatter {
  template <typename Context>
  auto format(const roq::Fill &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(external_trade_id="{}", )"
        R"(quantity={}, )"
        R"(price={}, )"
        R"(liquidity={})"
        R"(}})"_fmt,
        value.external_trade_id,
        value.quantity,
        value.price,
        value.liquidity);
  }
};
