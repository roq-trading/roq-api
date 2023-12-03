/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/compile.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/numbers.hpp"
#include "roq/option_type.hpp"
#include "roq/security_type.hpp"
#include "roq/trace.hpp"

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
  double min_notional = NaN;                      //!< Minimum notional (price * quantity)
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
  bool discard = false;                           //!< Discard market data updates?
};

template <>
inline constexpr std::string_view get_name<ReferenceData>() {
  using namespace std::literals;
  return "reference_data"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::ReferenceData> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::ReferenceData const &value, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
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
        R"(min_notional={}, )"
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
        R"(expiry_datetime_utc={}, )"
        R"(discard={})"
        R"(}})"_cf,
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
        value.min_notional,
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
        value.expiry_datetime_utc,
        value.discard);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::ReferenceData>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Event<roq::ReferenceData> const &event, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(reference_data={}, )"
        R"(message_info={})"
        R"(}})"_cf,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::ReferenceData>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Trace<roq::ReferenceData> const &event, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(reference_data={}, )"
        R"(trace_info={})"
        R"(}})"_cf,
        event.value,
        event.trace_info);
  }
};
