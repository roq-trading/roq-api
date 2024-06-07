/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <limits>

#include "roq/liquidity.hpp"
#include "roq/string_types.hpp"

namespace roq {

//! Represents a single fill (match) when an order is being partially or fully filled
struct ROQ_PUBLIC Fill final {
  std::chrono::nanoseconds exchange_time_utc = {};                        //!< Exchange timestamp, possibly from matching engine (UTC)
  roq::ExternalTradeId external_trade_id;                                 //!< External trade identifier
  double quantity = std::numeric_limits<double>::quiet_NaN();             //!< Quantity (base currency)
  double price = std::numeric_limits<double>::quiet_NaN();                //!< Price
  roq::Liquidity liquidity = {};                                          //!< Liquidity indicator
  double quote_quantity = std::numeric_limits<double>::quiet_NaN();       //!< Quantity (quote currency, optional)
  double commission_quantity = std::numeric_limits<double>::quiet_NaN();  //!< Quantity (commission currency, optional)
  roq::Currency commission_currency;                                      //!< Commission currency
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Fill> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Fill const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(exchange_time_utc={}, )"
        R"(external_trade_id="{}", )"
        R"(quantity={}, )"
        R"(price={}, )"
        R"(liquidity={}, )"
        R"(quote_quantity={}, )"
        R"(commission_quantity={}, )"
        R"(commission_currency="{}")"
        R"(}})"sv,
        value.exchange_time_utc,
        value.external_trade_id,
        value.quantity,
        value.price,
        value.liquidity,
        value.quote_quantity,
        value.commission_quantity,
        value.commission_currency);
  }
};
