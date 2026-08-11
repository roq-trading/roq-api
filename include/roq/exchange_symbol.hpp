/* Copyright (c) 2017-2026, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include <magic_enum/magic_enum_format.hpp>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/string_types.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Exchange and symbol
struct ROQ_PUBLIC ExchangeSymbol final {
  roq::Exchange exchange;  //!< Exchange
  roq::Symbol symbol;      //!< Symbol
};

template <>
constexpr std::string_view get_name<ExchangeSymbol>() {
  using namespace std::literals;
  return "exchange_symbol"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::ExchangeSymbol> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::ExchangeSymbol const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(exchange="{}", )"
        R"(symbol="{}")"
        R"(}})"sv,
        value.exchange,
        value.symbol);
  }
};