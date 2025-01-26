/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/core.h>

#include <magic_enum/magic_enum_format.hpp>

#include <string_view>

#include "roq/action.hpp"
#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Control service state
struct ROQ_PUBLIC Control final {
  roq::Action action = {};    //!< Action
  std::string_view user;      //!< User name (optional)
  uint32_t strategy_id = {};  //!< Strategy ID (optional)
  uint8_t leg = {};           //!< Leg index (optional)
  std::string_view account;   //!< Account (optional)
  std::string_view exchange;  //!< Exchange (optional)
  std::string_view symbol;    //!< Symbol (optional)
};

template <>
inline constexpr std::string_view get_name<Control>() {
  using namespace std::literals;
  return "control"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::Control> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Control const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(action={}, )"
        R"(user="{}", )"
        R"(strategy_id={}, )"
        R"(leg={}, )"
        R"(account="{}", )"
        R"(exchange="{}", )"
        R"(symbol="{}")"
        R"(}})"sv,
        value.action,
        value.user,
        value.strategy_id,
        value.leg,
        value.account,
        value.exchange,
        value.symbol);
  }
};
