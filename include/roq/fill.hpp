/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <magic_enum/magic_enum_format.hpp>

#include <chrono>

#include "roq/limits.hpp"
#include "roq/liquidity.hpp"
#include "roq/string_types.hpp"

namespace roq {

//! Represents a single fill (match) when an order is being partially or fully filled
struct ROQ_PUBLIC Fill final {
  std::chrono::nanoseconds exchange_time_utc = {};  //!< Exchange timestamp, possibly from matching engine (UTC)
  roq::ExternalTradeId external_trade_id;           //!< External trade identifier
  double quantity = roq::NaN;                       //!< Quantity
  double price = roq::NaN;                          //!< Price
  roq::Liquidity liquidity = {};                    //!< Liquidity indicator
  double base_amount = roq::NaN;                    //!< Funds (base currency, optional)
  double quote_amount = roq::NaN;                   //!< Funds (quote currency, optional)
  double commission_amount = roq::NaN;              //!< Funds (commission currency, optional)
  roq::Currency commission_currency;                //!< Commission currency
  double profit_loss_cost_amount = roq::NaN;        //!< P&L cost (settlement currency, internal)
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
        R"(base_amount={}, )"
        R"(quote_amount={}, )"
        R"(commission_amount={}, )"
        R"(commission_currency="{}", )"
        R"(profit_loss_cost_amount={})"
        R"(}})"sv,
        value.exchange_time_utc,
        value.external_trade_id,
        value.quantity,
        value.price,
        value.liquidity,
        value.base_amount,
        value.quote_amount,
        value.commission_amount,
        value.commission_currency,
        value.profit_loss_cost_amount);
  }
};
