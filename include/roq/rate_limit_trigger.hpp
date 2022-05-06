/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/compat.hpp"
#include "roq/event.hpp"
#include "roq/mask.hpp"
#include "roq/message_info.hpp"
#include "roq/name.hpp"
#include "roq/numbers.hpp"
#include "roq/span.hpp"
#include "roq/string_types.hpp"
#include "roq/trace.hpp"

#include "roq/origin.hpp"
#include "roq/rate_limit_type.hpp"

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
  auto format(const roq::RateLimitTrigger &value, Context &context) {
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
        R"(triggered_by="{}")"
        R"(}})"sv,
        value.name,
        value.origin,
        value.type,
        fmt::join(value.users, ", "sv),
        fmt::join(value.accounts, ", "sv),
        value.ban_expires,
        value.triggered_by);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::RateLimitTrigger> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::Event<roq::RateLimitTrigger> &event, Context &context) {
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
struct fmt::formatter<roq::Trace<roq::RateLimitTrigger const> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::Trace<roq::RateLimitTrigger const> &event, Context &context) {
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
