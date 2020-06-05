/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>

#include "roq/compat.h"

namespace roq {
namespace client {

struct ROQ_PUBLIC TimerEvent final {
  std::chrono::nanoseconds now;  // monotonic clock
};

}  // namespace client
}  // namespace roq

template <>
struct fmt::formatter<roq::client::TimerEvent> {
  template <typename Context>
  constexpr auto parse(Context& context) {
    return context.begin();
  }
  template <typename Context>
  auto format(
      const roq::client::TimerEvent& value,
      Context& context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(now={})"
        R"(}})",
        value.now);
  }
};
