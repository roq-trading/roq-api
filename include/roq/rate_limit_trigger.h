/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/string_buffer.h"

#include "roq/origin.h"
#include "roq/rate_limit_type.h"

namespace roq {

//! Rate-limit trigger
struct ROQ_PUBLIC RateLimitTrigger final {
  std::string_view name;                             //!< Configuration name
  Origin origin = {};                                //!< Origin
  RateLimitType type = {};                           //!< Rate-limit type
  roq::span<roq::string_buffer<16> const> users;     //!< Sorted list of users being affected (empty list means: all)
  roq::span<roq::string_buffer<32> const> accounts;  //!< Sorted list of accounts being affected (empty list means: all)
  std::chrono::nanoseconds ban_expires = {};  //!< System time when ban expires (zero means: ban is no longer effective)
  std::string_view triggered_by;              //!< Trigger activated by this user
};

}  // namespace roq

template <>
struct fmt::formatter<roq::RateLimitTrigger> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::RateLimitTrigger &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
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
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::Event<roq::RateLimitTrigger> &event, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(rate_limit_trigger={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
