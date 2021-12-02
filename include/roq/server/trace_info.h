/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

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
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::server::TraceInfo &value, Context &ctx) {
    using namespace std::literals::string_view_literals;
    return fmt::format_to(
        ctx.out(),
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
