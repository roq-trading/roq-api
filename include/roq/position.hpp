/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"
#include "roq/compat/fmt.hpp"

#include <fmt/format.h>

#include <magic_enum/magic_enum_format.hpp>

#include "roq/limits.hpp"
#include "roq/string_types.hpp"

namespace roq {

//! Position
struct ROQ_PUBLIC Position final {
  roq::Exchange exchange;              //!< Exchange
  roq::Symbol symbol;                  //!< Symbol
  double position = 0.0;               //!< Position (quantity)
  double profit_loss_amount = 0.0;     //!< P&L (internal)
  roq::Currency profit_loss_currency;  //!< P&L currency
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Position> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Position const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(position={}, )"
        R"(profit_loss_amount={}, )"
        R"(profit_loss_currency="{}")"
        R"(}})"sv,
        value.exchange,
        value.symbol,
        value.position,
        value.profit_loss_amount,
        value.profit_loss_currency);
  }
};