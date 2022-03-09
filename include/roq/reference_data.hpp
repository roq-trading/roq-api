/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/compat.hpp"
#include "roq/event.hpp"
#include "roq/message_info.hpp"
#include "roq/numbers.hpp"
#include "roq/string_buffer.hpp"

#include "roq/option_type.hpp"
#include "roq/security_type.hpp"

namespace roq {

//! Update relating to the reference data for a symbol
struct ROQ_PUBLIC ReferenceData final {
  uint16_t stream_id = {};                        //!< Stream identifier
  std::string_view exchange;                      //!< Exchange
  std::string_view symbol;                        //!< Symbol
  std::string_view description;                   //!< Description
  SecurityType security_type = {};                //!< Security type
  std::string_view base_currency;                 //!< Base currency
  std::string_view quote_currency;                //!< Quote currency
  std::string_view margin_currency;               //!< Margin currency
  std::string_view commission_currency;           //!< Commission currency
  double tick_size = NaN;                         //!< Minimum price increment
  double multiplier = NaN;                        //!< Multiplier (notional)
  double min_trade_vol = NaN;                     //!< Minimum trade volume
  double max_trade_vol = NaN;                     //!< Maximum trade volume
  double trade_vol_step_size = NaN;               //!< Trade volume step size
  OptionType option_type = {};                    //!< Option type
  std::string_view strike_currency;               //!< Strike currency
  double strike_price = NaN;                      //!< Strike price
  std::string_view underlying;                    //!< Underlying instrument
  std::string_view time_zone;                     //!< Time-zone
  std::chrono::days issue_date = {};              //!< Issue date
  std::chrono::days settlement_date = {};         //!< Settlement date
  std::chrono::seconds expiry_datetime = {};      //!< Expiry datetime
  std::chrono::seconds expiry_datetime_utc = {};  //!< Expiry datetime
};

}  // namespace roq

template <>
struct fmt::formatter<roq::ReferenceData> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::ReferenceData &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(description="{}", )"
        R"(security_type={}, )"
        R"(base_currency="{}", )"
        R"(quote_currency="{}", )"
        R"(margin_currency="{}", )"
        R"(commission_currency="{}", )"
        R"(tick_size={}, )"
        R"(multiplier={}, )"
        R"(min_trade_vol={}, )"
        R"(max_trade_vol={}, )"
        R"(trade_vol_step_size={}, )"
        R"(option_type={}, )"
        R"(strike_currency="{}", )"
        R"(strike_price={}, )"
        R"(underlying="{}", )"
        R"(time_zone="{}", )"
        R"(issue_date={}, )"
        R"(settlement_date={}, )"
        R"(expiry_datetime={}, )"
        R"(expiry_datetime_utc={})"
        R"(}})"sv,
        value.stream_id,
        value.exchange,
        value.symbol,
        value.description,
        value.security_type,
        value.base_currency,
        value.quote_currency,
        value.margin_currency,
        value.commission_currency,
        value.tick_size,
        value.multiplier,
        value.min_trade_vol,
        value.max_trade_vol,
        value.trade_vol_step_size,
        value.option_type,
        value.strike_currency,
        value.strike_price,
        value.underlying,
        value.time_zone,
        value.issue_date,
        value.settlement_date,
        value.expiry_datetime,
        value.expiry_datetime_utc);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::ReferenceData> > {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::Event<roq::ReferenceData> &event, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(reference_data={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
