/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/compat.h"
#include "roq/fixed_string.h"
#include "roq/span.h"

#include "roq/event.h"
#include "roq/message_info.h"

namespace roq {

//! Represents a timer update
struct ROQ_PUBLIC Timer final {
  Timer() = default;
  Timer(Timer &&) = default;
  Timer(const Timer &) = delete;

  std::chrono::nanoseconds now = {};  //!< Current time (monotonic clock)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Timer> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Timer &value, Context &context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(now={})"
        R"(}})",
        value.now);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::Timer> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Event<roq::Timer> &event, Context &context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(timer={})"
        R"(}})",
        event.message_info,
        event.value);
  }
};
