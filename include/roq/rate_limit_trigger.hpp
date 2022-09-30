/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/buffer_capacity.hpp"
#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/origin.hpp"
#include "roq/rate_limit_type.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Rate-limit trigger
struct ROQ_PUBLIC RateLimitTrigger final {
  std::string_view name;                      //!< Configuration name
  Origin origin = {};                         //!< Origin
  RateLimitType type = {};                    //!< Rate-limit type
  std::span<User const> users;                //!< Sorted list of users being affected (empty list means: all)
  std::span<Account const> accounts;          //!< Sorted list of accounts being affected (empty list means: all)
  std::chrono::nanoseconds ban_expires = {};  //!< System time when ban expires (zero means: ban is no longer effective)
  std::string_view triggered_by;              //!< Trigger activated by this user
  BufferCapacity buffer_capacity = {};        //!< Buffer capacity (indicator for how full or empty the buffer is)
  uint32_t remaining_requests = {};           //!< The buffer becomes full if this many requests are sent instantly
};

template <>
inline constexpr std::string_view get_name<RateLimitTrigger>() {
  using namespace std::literals;
  return "rate_limit_trigger"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::RateLimitTrigger> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::RateLimitTrigger const &value, Context &context) const {
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

template <>
struct fmt::formatter<roq::Event<roq::RateLimitTrigger> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Event<roq::RateLimitTrigger> const &event, Context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(rate_limit_trigger={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::RateLimitTrigger> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Trace<roq::RateLimitTrigger> const &event, Context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(rate_limit_trigger={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
