/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <magic_enum/magic_enum_format.hpp>

#include <chrono>

#include "roq/limits.hpp"

namespace roq {

//! Represents a single bar of a time-series
struct ROQ_PUBLIC Bar final {
  std::chrono::nanoseconds begin_time_utc = {};  //!< Begin-of-period time-stamp (UTC)
  double open_price = roq::NaN;                  //!< Open price
  double high_price = roq::NaN;                  //!< High price
  double low_price = roq::NaN;                   //!< Low price
  double close_price = roq::NaN;                 //!< Close price
  double quantity = roq::NaN;                    //!< Total quantity (contracts or base currency)
  double base_amount = roq::NaN;                 //!< Total amount (base currency)
  double quote_amount = roq::NaN;                //!< Total amount (quote currency)
  uint32_t number_of_trades = {};                //!< Number of trades (count)
  double vwap = roq::NaN;                        //!< VWAP
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Bar> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Bar const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(begin_time_utc={}, )"
        R"(open_price={}, )"
        R"(high_price={}, )"
        R"(low_price={}, )"
        R"(close_price={}, )"
        R"(quantity={}, )"
        R"(base_amount={}, )"
        R"(quote_amount={}, )"
        R"(number_of_trades={}, )"
        R"(vwap={})"
        R"(}})"sv,
        value.begin_time_utc,
        value.open_price,
        value.high_price,
        value.low_price,
        value.close_price,
        value.quantity,
        value.base_amount,
        value.quote_amount,
        value.number_of_trades,
        value.vwap);
  }
};
