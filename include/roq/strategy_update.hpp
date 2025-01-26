/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <magic_enum/magic_enum_format.hpp>

#include <span>
#include <string_view>

#include "roq/event.hpp"
#include "roq/leg.hpp"
#include "roq/name.hpp"
#include "roq/state.hpp"
#include "roq/trace.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Strategy update
struct ROQ_PUBLIC StrategyUpdate final {
  std::string_view user;             //!< Service name
  uint32_t strategy_id = {};         //!< Strategy ID
  std::string_view description;      //!< Strategy description
  roq::State state = {};             //!< Strategy state
  std::span<roq::Leg const> legs;    //!< List of leg updates
  roq::UpdateType update_type = {};  //!< Update type
};

template <>
inline constexpr std::string_view get_name<StrategyUpdate>() {
  using namespace std::literals;
  return "strategy_update"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::StrategyUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::StrategyUpdate const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(user="{}", )"
        R"(strategy_id={}, )"
        R"(description="{}", )"
        R"(state={}, )"
        R"(legs=[{}], )"
        R"(update_type={})"
        R"(}})"sv,
        value.user,
        value.strategy_id,
        value.description,
        value.state,
        fmt::join(value.legs, ", "sv),
        value.update_type);
  }
};
