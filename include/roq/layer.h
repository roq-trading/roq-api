/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/chrono.h"
#include "roq/fixed_string.h"
#include "roq/format.h"
#include "roq/literals.h"
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
struct fmt::formatter<roq::Layer> : public roq::formatter {
  template <typename Context>
  auto format(const roq::Layer &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(bid_price={}, )"
        R"(bid_quantity={}, )"
        R"(ask_price={}, )"
        R"(ask_quantity={})"
        R"(}})"_fmt,
        value.bid_price,
        value.bid_quantity,
        value.ask_price,
        value.ask_quantity);
  }
};
