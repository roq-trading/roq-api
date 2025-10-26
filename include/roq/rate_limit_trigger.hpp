/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include <magic_enum/magic_enum_format.hpp>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/buffer_capacity.hpp"
#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/origin.hpp"
#include "roq/rate_limit_type.hpp"
#include "roq/string_types.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Rate-limit trigger
struct ROQ_PUBLIC RateLimitTrigger final {
  std::string_view name;                      //!< Configuration name
  roq::Origin origin = {};                    //!< Origin
  roq::RateLimitType type = {};               //!< Rate-limit type
  std::span<roq::User const> users;           //!< Sorted list of users being affected (empty list means: all)
  std::span<roq::Account const> accounts;     //!< Sorted list of accounts being affected (empty list means: all)
  std::chrono::nanoseconds ban_expires = {};  //!< System time when ban expires (zero means: ban is no longer effective)
  std::string_view triggered_by;              //!< Trigger activated by this user
  roq::BufferCapacity buffer_capacity = {};   //!< Buffer capacity (indicator for how full or empty the buffer is)
  uint32_t remaining_requests = {};           //!< The buffer becomes full if this many requests are sent instantly
};

template <>
constexpr std::string_view get_name<RateLimitTrigger>() {
  using namespace std::literals;
  return "rate_limit_trigger"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::RateLimitTrigger> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::RateLimitTrigger const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(name="{}", )"
        R"(origin={}, )"
        R"(type={}, )"
        R"(users=[{}], )"
        R"(accounts=[{}], )"
        R"(ban_expires={}, )"
        R"(triggered_by="{}", )"
        R"(buffer_capacity={}, )"
        R"(remaining_requests={})"
        R"(}})"sv,
        value.name,
        value.origin,
        value.type,
        fmt::join(value.users, ", "sv),
        fmt::join(value.accounts, ", "sv),
        value.ban_expires,
        value.triggered_by,
        value.buffer_capacity,
        value.remaining_requests);
  }
};