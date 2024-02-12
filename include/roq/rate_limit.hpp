/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/rate_limit_type.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Rate-limit
struct ROQ_PUBLIC RateLimit final {
  RateLimitType type = {};                 //!< Type
  std::chrono::seconds period = {};        //!< Monitor period
  std::chrono::seconds end_time_utc = {};  //!< End of current period
  uint32_t limit = {};                     //!< Limit
  uint32_t count = {};                     //!< Count
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
        R"(count={})"
        R"(}})"sv,
        value.type,
        value.period,
        value.end_time_utc,
        value.limit,
        value.count);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::RateLimit>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::RateLimit> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(rate_limit={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::RateLimit>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::RateLimit> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(rate_limit={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
