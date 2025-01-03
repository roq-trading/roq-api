/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/core.h>

#include <magic_enum/magic_enum_format.hpp>

#include "roq/string_types.hpp"

namespace roq {

//! Represents a single parameter
struct ROQ_PUBLIC Parameter final {
  roq::ParameterKey label;    //!< Label
  uint32_t strategy_id = {};  //!< Strategy identifier (optional)
  roq::Account account;       //!< Account name
  roq::Exchange exchange;     //!< Exchange
  roq::Symbol symbol;         //!< Symbol
  roq::ParameterValue value;  //!< Value
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Parameter> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Parameter const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(label="{}", )"
        R"(strategy_id={}, )"
        R"(account="{}", )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(value="{}")"
        R"(}})"sv,
        value.label,
        value.strategy_id,
        value.account,
        value.exchange,
        value.symbol,
        value.value);
  }
};
