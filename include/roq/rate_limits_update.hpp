/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <span>
#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/origin.hpp"
#include "roq/rate_limit.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Rate-limits update
struct ROQ_PUBLIC RateLimitsUpdate final {
  uint16_t stream_id = {};                      //!< Stream identifier
  std::string_view account;                     //!< Account name
  roq::Origin origin = {};                      //!< Origin
  std::span<roq::RateLimit const> rate_limits;  //!< Rate-limits updates
};

template <>
inline constexpr std::string_view get_name<RateLimitsUpdate>() {
  using namespace std::literals;
  return "rate_limits_update"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::RateLimitsUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::RateLimitsUpdate const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(account="{}", )"
        R"(origin={}, )"
        R"(rate_limits=[{}])"
        R"(}})"sv,
        value.stream_id,
        value.account,
        value.origin,
        fmt::join(value.rate_limits, ", "sv));
  }
};

template <>
struct fmt::formatter<roq::Event<roq::RateLimitsUpdate>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::RateLimitsUpdate> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(rate_limits_update={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::RateLimitsUpdate>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::RateLimitsUpdate> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(rate_limits_update={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
