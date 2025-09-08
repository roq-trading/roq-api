/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"
#include "roq/compat/fmt.hpp"

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <magic_enum/magic_enum_format.hpp>

#include <span>
#include <string_view>

#include "roq/event.hpp"
#include "roq/leg.hpp"
#include "roq/name.hpp"
#include "roq/trace.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Legs update
struct ROQ_PUBLIC LegsUpdate final {
  std::string_view user;             //!< Service name
  uint32_t strategy_id = {};         //!< Strategy ID
  std::span<roq::Leg const> legs;    //!< List of leg updates
  roq::UpdateType update_type = {};  //!< Update type
};

template <>
constexpr std::string_view get_name<LegsUpdate>() {
  using namespace std::literals;
  return "legs_update"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::LegsUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::LegsUpdate const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(user="{}", )"
        R"(strategy_id={}, )"
        R"(legs=[{}], )"
        R"(update_type={})"
        R"(}})"sv,
        value.user,
        value.strategy_id,
        fmt::join(value.legs, ", "sv),
        value.update_type);
  }
};