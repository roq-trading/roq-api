/* Copyright (c) 2017-2023, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/compile.h>
#include <fmt/format.h>

#include "roq/liquidity.hpp"
#include "roq/numbers.hpp"
#include "roq/string_types.hpp"

namespace roq {

//! Represents a single fill (match) when an order is being partially or fully filled
struct ROQ_PUBLIC Fill final {
  ExternalTradeId external_trade_id;  //!< External trade identifier
  double quantity = NaN;              //!< Quantity
  double price = NaN;                 //!< Price
  Liquidity liquidity = {};           //!< Liquidity indicator
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Fill> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Fill const &value, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(external_trade_id="{}", )"
        R"(quantity={}, )"
        R"(price={}, )"
        R"(liquidity={})"
        R"(}})"_cf,
        value.external_trade_id,
        value.quantity,
        value.price,
        value.liquidity);
  }
};
