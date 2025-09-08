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
#include "roq/name.hpp"
#include "roq/risk_limit.hpp"
#include "roq/trace.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Risk limits (publish) !!! EXPERIMENTAL !!!
struct ROQ_PUBLIC RiskLimitsUpdate final {
  std::string_view user;                   //!< User name (optional)
  uint32_t strategy_id = {};               //!< Strategy identifier (optional)
  std::string_view account;                //!< Account name (optional)
  std::span<roq::RiskLimit const> limits;  //!< Risk limits per {exchange, symbol}
  roq::UpdateType update_type = {};        //!< Update type
};

template <>
constexpr std::string_view get_name<RiskLimitsUpdate>() {
  using namespace std::literals;
  return "risk_limits_update"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::RiskLimitsUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::RiskLimitsUpdate const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(user="{}", )"
        R"(strategy_id={}, )"
        R"(account="{}", )"
        R"(limits=[{}], )"
        R"(update_type={})"
        R"(}})"sv,
        value.user,
        value.strategy_id,
        value.account,
        fmt::join(value.limits, ", "sv),
        value.update_type);
  }
};