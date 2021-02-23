/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/fixed_string.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"

#include "roq/option_type.h"
#include "roq/security_type.h"

namespace roq {

//! Update relating to the reference data for a symbol
struct ROQ_PUBLIC ReferenceData final {
  std::string_view exchange;                             //!< Exchange name
  std::string_view symbol;                               //!< Symbol
  std::string_view description;                          //!< Description
  SecurityType security_type = SecurityType::UNDEFINED;  //!< Security type
  std::string_view currency;                             //!< Currency name
  std::string_view settlement_currency;                  //!< Currency name
  std::string_view commission_currency;                  //!< Currency name
  double tick_size = NaN;                                //!< Minimum price increment
  double multiplier = NaN;                               //!< Multiplier (notional)
  double min_trade_vol = NaN;                            //!< Minimum trade volume
  OptionType option_type = OptionType::UNDEFINED;        //!< Option type
  std::string_view strike_currency;                      //!< Strike currency
  double strike_price = NaN;                             //!< Strike price
  std::string_view underlying;                           //!< Underlying instrument
  std::string_view time_zone;                            //!< Time-zone
  roq::chrono::days issue_date = {};                     //!< Issue date
  roq::chrono::days settlement_date = {};                //!< Settlement date
  std::chrono::seconds expiry_datetime = {};             //!< Expiry datetime
  std::chrono::seconds expiry_datetime_utc = {};         //!< Expiry datetime
};

}  // namespace roq

template <>
struct fmt::formatter<roq::ReferenceData> : public roq::formatter {
  template <typename Context>
  auto format(const roq::ReferenceData &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(description="{}", )"
        R"(security_type={}, )"
        R"(currency="{}", )"
        R"(settlement_currency="{}", )"
        R"(commission_currency="{}", )"
        R"(tick_size={}, )"
        R"(multiplier={}, )"
        R"(min_trade_vol={}, )"
        R"(option_type={}, )"
        R"(strike_currency="{}", )"
        R"(strike_price={}, )"
        R"(underlying="{}", )"
        R"(time_zone="{}", )"
        R"(issue_date={}, )"
        R"(settlement_date={}, )"
        R"(expiry_datetime={}, )"
        R"(expiry_datetime_utc={})"
        R"(}})"_fmt,
        value.exchange,
        value.symbol,
        value.description,
        value.security_type,
        value.currency,
        value.settlement_currency,
        value.commission_currency,
        value.tick_size,
        value.multiplier,
        value.min_trade_vol,
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
struct fmt::formatter<roq::Event<roq::ReferenceData> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::ReferenceData> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(reference_data={})"
        R"(}})"_fmt,
        event.message_info,
        event.value);
  }
};
