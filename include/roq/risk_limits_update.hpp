/* Copyright (c) 2017-2023, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/compile.h>
#include <fmt/format.h>

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
  std::string_view account;           //!< Account name (optional)
  std::string_view user;              //!< User name (optional)
  uint32_t strategy = {};             //!< Strategy (optional)
  std::span<RiskLimit const> limits;  //!< Risk limits per {exchange, symbol}
  UpdateType update_type = {};        //!< Update type
};

template <>
inline constexpr std::string_view get_name<RiskLimitsUpdate>() {
  using namespace std::literals;
  return "risk_limits_update"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::RiskLimitsUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::RiskLimitsUpdate const &value, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(account="{}", )"
        R"(user="{}", )"
        R"(strategy={}, )"
        R"(limits=[{}], )"
        R"(update_type={})"
        R"(}})"_cf,
        value.account,
        value.user,
        value.strategy,
        fmt::join(value.limits, ", "sv),
        value.update_type);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::RiskLimitsUpdate>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Event<roq::RiskLimitsUpdate> const &event, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(risk_limits_update={}, )"
        R"(message_info={})"
        R"(}})"_cf,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::RiskLimitsUpdate>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Trace<roq::RiskLimitsUpdate> const &event, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(risk_limits_update={}, )"
        R"(trace_info={})"
        R"(}})"_cf,
        event.value,
        event.trace_info);
  }
};
