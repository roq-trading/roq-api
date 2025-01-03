/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/core.h>
#include <fmt/ranges.h>

#include <magic_enum/magic_enum_format.hpp>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/event.hpp"
#include "roq/limits.hpp"
#include "roq/name.hpp"
#include "roq/option_type.hpp"
#include "roq/security_type.hpp"
#include "roq/tick_size_step.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Update relating to the reference data for a symbol
struct ROQ_PUBLIC ReferenceData final {
  uint16_t stream_id = {};                             //!< Stream identifier
  std::string_view exchange;                           //!< Exchange
  std::string_view symbol;                             //!< Symbol
  std::string_view description;                        //!< Description
  roq::SecurityType security_type = {};                //!< Security type
  std::string_view cfi_code;                           //!< CFI code
  std::string_view base_currency;                      //!< Base currency
  std::string_view quote_currency;                     //!< Quote currency
  std::string_view settlement_currency;                //!< Settlement currency
  std::string_view margin_currency;                    //!< Margin currency
  std::string_view commission_currency;                //!< Commission currency
  double tick_size = roq::NaN;                         //!< Minimum price increment
  std::span<roq::TickSizeStep const> tick_size_steps;  //!< List of tick size steps
  double multiplier = roq::NaN;                        //!< Multiplier (notional)
  double min_notional = roq::NaN;                      //!< Minimum notional (price * quantity)
  double min_trade_vol = roq::NaN;                     //!< Minimum trade volume
  double max_trade_vol = roq::NaN;                     //!< Maximum trade volume
  double trade_vol_step_size = roq::NaN;               //!< Trade volume step size
  roq::OptionType option_type = {};                    //!< Option type
  std::string_view strike_currency;                    //!< Strike currency
  double strike_price = roq::NaN;                      //!< Strike price
  std::string_view underlying;                         //!< Underlying instrument
  std::string_view time_zone;                          //!< Time-zone
  std::chrono::days issue_date = {};                   //!< Issue date
  std::chrono::days settlement_date = {};              //!< Settlement date
  std::chrono::seconds expiry_datetime = {};           //!< Expiry datetime
  std::chrono::seconds expiry_datetime_utc = {};       //!< Expiry datetime
  std::chrono::nanoseconds exchange_time_utc = {};     //!< Exchange timestamp, possibly from matching engine (UTC)
  uint64_t exchange_sequence = {};                     //!< Exchange message sequence number
  std::chrono::nanoseconds sending_time_utc = {};      //!< Exchange sending timestamp (UTC)
  bool discard = false;                                //!< Discard market data updates?
};

template <>
inline constexpr std::string_view get_name<ReferenceData>() {
  using namespace std::literals;
  return "reference_data"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::ReferenceData> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::ReferenceData const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(description="{}", )"
        R"(security_type={}, )"
        R"(cfi_code="{}", )"
        R"(base_currency="{}", )"
        R"(quote_currency="{}", )"
        R"(settlement_currency="{}", )"
        R"(margin_currency="{}", )"
        R"(commission_currency="{}", )"
        R"(tick_size={}, )"
        R"(tick_size_steps=[{}], )"
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
        R"(exchange_time_utc={}, )"
        R"(exchange_sequence={}, )"
        R"(sending_time_utc={}, )"
        R"(discard={})"
        R"(}})"sv,
        value.stream_id,
        value.exchange,
        value.symbol,
        value.description,
        value.security_type,
        value.cfi_code,
        value.base_currency,
        value.quote_currency,
        value.settlement_currency,
        value.margin_currency,
        value.commission_currency,
        value.tick_size,
        fmt::join(value.tick_size_steps, ", "sv),
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
        value.exchange_time_utc,
        value.exchange_sequence,
        value.sending_time_utc,
        value.discard);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::ReferenceData>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::ReferenceData> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(reference_data={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::ReferenceData>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::ReferenceData> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(reference_data={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
