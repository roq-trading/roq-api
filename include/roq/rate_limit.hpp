/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/core.h>

#include <magic_enum/magic_enum_format.hpp>

#include <chrono>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/rate_limit_type.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Rate-limit
struct ROQ_PUBLIC RateLimit final {
  roq::RateLimitType type = {};            //!< Type
  std::chrono::seconds period = {};        //!< Monitor period
  std::chrono::seconds end_time_utc = {};  //!< End of current period
  uint32_t limit = {};                     //!< Limit
  uint32_t value = {};                     //!< Value
};

template <>
inline constexpr std::string_view get_name<RateLimit>() {
  using namespace std::literals;
  return "rate_limit"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::RateLimit> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::RateLimit const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(type={}, )"
        R"(period={}, )"
        R"(end_time_utc={}, )"
        R"(limit={}, )"
        R"(value={})"
        R"(}})"sv,
        value.type,
        value.period,
        value.end_time_utc,
        value.limit,
        value.value);
  }
};
