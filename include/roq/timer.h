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

namespace roq {

//! Represents a timer update
struct ROQ_PUBLIC Timer final {
  std::chrono::nanoseconds now = {};  //!< Current time (monotonic clock)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Timer> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::Timer &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(now={})"
        R"(}})"sv,
        value.now);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::Timer> > {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::Event<roq::Timer> &event, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(timer={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
