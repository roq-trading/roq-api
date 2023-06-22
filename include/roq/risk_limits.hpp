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

namespace roq {

//! Risk limits (receive)
struct ROQ_PUBLIC RiskLimits final {
  std::string_view label;             //!< Label
  std::string_view account;           //!< Account name (optional)
  std::string_view user;              //!< User name (optional)
  std::span<RiskLimit const> limits;  //!< Risk limits per {exchange, symbol}
  uint64_t seqno = {};                //!< Reference (sequencing)
};

template <>
inline constexpr std::string_view get_name<RiskLimits>() {
  using namespace std::literals;
  return "risk_limits"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::RiskLimits> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::RiskLimits const &value, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(label="{}", )"
        R"(account="{}", )"
        R"(user="{}", )"
        R"(limits=[{}], )"
        R"(seqno={})"
        R"(}})"_cf,
        value.label,
        value.account,
        value.user,
        fmt::join(value.limits, ", "sv),
        value.seqno);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::RiskLimits>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Event<roq::RiskLimits> const &event, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(risk_limits={}, )"
        R"(message_info={})"
        R"(}})"_cf,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::RiskLimits>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Trace<roq::RiskLimits> const &event, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(risk_limits={}, )"
        R"(trace_info={})"
        R"(}})"_cf,
        event.value,
        event.trace_info);
  }
};
