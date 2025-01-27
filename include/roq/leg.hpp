/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/core.h>

#include <magic_enum/magic_enum_format.hpp>

#include "roq/state.hpp"
#include "roq/string_types.hpp"

namespace roq {

//! Represents current state of a strategy leg
struct ROQ_PUBLIC Leg final {
  uint16_t leg_id = {};    //!< Index
  roq::Account account;    //!< Account (optional)
  roq::Exchange exchange;  //!< Exchange (optional)
  roq::Symbol symbol;      //!< Symbol (optional)
  roq::State state = {};   //!< Strategy state
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Leg> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Leg const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(leg_id={}, )"
        R"(account="{}", )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(state={})"
        R"(}})"sv,
        value.leg_id,
        value.account,
        value.exchange,
        value.symbol,
        value.state);
  }
};
