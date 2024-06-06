/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include <limits>

#include "roq/string_types.hpp"

namespace roq {

//! Risk limit for {exchange, symbol}
struct ROQ_PUBLIC RiskLimit final {
  roq::Exchange exchange;                                                       //!< Exchange
  roq::Symbol symbol;                                                           //!< Symbol
  double long_position = 0.0;                                                   //!< Position (long)
  double short_position = 0.0;                                                  //!< Position (short)
  double long_position_limit = std::numeric_limits<double>::quiet_NaN();        //!< Position limit (long)
  double short_position_limit = std::numeric_limits<double>::quiet_NaN();       //!< Position limit (short)
  double long_risk_exposure_limit = std::numeric_limits<double>::quiet_NaN();   //!< Risk exposure limit (long)
  double short_risk_exposure_limit = std::numeric_limits<double>::quiet_NaN();  //!< Risk exposure limit (short)
  bool allow_netting = false;                                                   //!< Allow netting?
};

}  // namespace roq

template <>
struct fmt::formatter<roq::RiskLimit> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::RiskLimit const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(long_position={}, )"
        R"(short_position={}, )"
        R"(long_position_limit={}, )"
        R"(short_position_limit={}, )"
        R"(long_risk_exposure_limit={}, )"
        R"(short_risk_exposure_limit={}, )"
        R"(allow_netting={})"
        R"(}})"sv,
        value.exchange,
        value.symbol,
        value.long_position,
        value.short_position,
        value.long_position_limit,
        value.short_position_limit,
        value.long_risk_exposure_limit,
        value.short_risk_exposure_limit,
        value.allow_netting);
  }
};
