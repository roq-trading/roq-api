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

//! Service control
struct ROQ_PUBLIC Control final {
  std::string_view request_id;  //!< Request identifier
  std::string_view user;        //!< User name (client routing, optional)
  roq::Action action = {};      //!< Action
  uint32_t strategy_id = {};    //!< Strategy filter (optional)
  uint16_t leg_id = {};         //!< Leg index filter (optional)
  std::string_view account;     //!< Account filter (optional)
  std::string_view exchange;    //!< Exchange filter (optional)
  std::string_view symbol;      //!< Symbol filter (optional)
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
        R"(request_id="{}", )"
        R"(user="{}", )"
        R"(action={}, )"
        R"(strategy_id={}, )"
        R"(leg_id={}, )"
        R"(account="{}", )"
        R"(exchange="{}", )"
        R"(symbol="{}")"
        R"(}})"sv,
        value.request_id,
        value.user,
        value.action,
        value.strategy_id,
        value.leg_id,
        value.account,
        value.exchange,
        value.symbol);
  }
};
