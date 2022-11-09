/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include "roq/string_types.hpp"

namespace roq {

//! Represents a single parameter
struct ROQ_PUBLIC Parameter final {
  ParameterKey label;    //!< Label
  Account account;       //!< Account name
  Exchange exchange;     //!< Exchange
  Symbol symbol;         //!< Symbol
  ParameterValue value;  //!< Value
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Parameter> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Parameter const &value, Context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(label="{}", )"
        R"(account="{}", )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(value="{}")"
        R"(}})"sv,
        value.label,
        value.account,
        value.exchange,
        value.symbol,
        value.value);
  }
};
