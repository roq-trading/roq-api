/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <string_view>

#include <fmt/chrono.h>
#include <fmt/format.h>

#include "roq/compat.h"

namespace roq {
namespace server {

struct ROQ_PUBLIC TraceInfo final {
  TraceInfo() = delete;

  std::chrono::nanoseconds source_receive_time = {};
  std::chrono::nanoseconds origin_create_time = {};
  std::chrono::nanoseconds origin_create_time_utc = {};
};

}  // namespace server
}  // namespace roq

template <>
struct fmt::formatter<roq::server::TraceInfo> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::server::TraceInfo &value, Context &context) {
    using namespace std::literals::string_view_literals;
    return fmt::format_to(
        context.out(),
        "{{"
        "source_receive_time={}, "
        "origin_create_time={}, "
        "origin_create_time_utc={}"
        "}}"sv,
        value.source_receive_time,
        value.origin_create_time,
        value.origin_create_time_utc);
  }
};
