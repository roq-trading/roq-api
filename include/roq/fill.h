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
#include "roq/span.h"
#include "roq/uuid.h"

namespace roq {

//! Represents a single fill (match) when an order is being partially or fully filled
struct ROQ_PUBLIC Fill final {
  double quantity = std::numeric_limits<double>::quiet_NaN();  //!< Quantity
  double price = std::numeric_limits<double>::quiet_NaN();     //!< Price
  uint32_t trade_id = 0;          //!< Trade identifier (as known to client)
  uint32_t gateway_trade_id = 0;  //!< Trade identifier (as known to gateway)
  roq::fixed_string<40>
      external_trade_id;  //!< External trade identifier (as known to broker or exchange)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Fill> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Fill &value, Context &context) {
    using namespace std::literals;  // NOLINT
    return format_to(
        context.out(),
        R"({{)"
        R"(quantity={}, )"
        R"(price={}, )"
        R"(trade_id={}, )"
        R"(gateway_trade_id={}, )"
        R"(external_trade_id="{}")"
        R"(}})"sv,
        value.quantity,
        value.price,
        value.trade_id,
        value.gateway_trade_id,
        value.external_trade_id);
  }
};
