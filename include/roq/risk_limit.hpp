/* Copyright (c) 2017-2023, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/compile.h>
#include <fmt/format.h>

#include "roq/numbers.hpp"
#include "roq/string_types.hpp"

namespace roq {

//! Risk limit for {exchange, symbol}
struct ROQ_PUBLIC RiskLimit final {
  Exchange exchange;                       //!< Exchange
  Symbol symbol;                           //!< Symbol
  double long_position = 0.0;              //!< Position (long)
  double short_position = 0.0;             //!< Position (short)
  double long_position_limit = NaN;        //!< Position limit (long)
  double short_position_limit = NaN;       //!< Position limit (short)
  double long_risk_exposure_limit = NaN;   //!< Risk exposure limit (long)
  double short_risk_exposure_limit = NaN;  //!< Risk exposure limit (short)
  bool allow_netting = false;              //!< Allow netting?
};

}  // namespace roq

template <>
struct fmt::formatter<roq::RiskLimit> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::RiskLimit const &value, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
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
        R"(}})"_cf,
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
