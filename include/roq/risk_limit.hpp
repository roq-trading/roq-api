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
  Exchange exchange;        //!< Exchange
  Symbol symbol;            //!< Symbol
  double buy_limit = NaN;   //!< Buy limit
  double sell_limit = NaN;  //!< Sell limit
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
        R"(buy_limit={}, )"
        R"(sell_limit={})"
        R"(}})"_cf,
        value.exchange,
        value.symbol,
        value.buy_limit,
        value.sell_limit);
  }
};
