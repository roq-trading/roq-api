/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/compat.hpp"

namespace roq {

struct ROQ_PUBLIC TraceInfo final {
  std::chrono::nanoseconds source_receive_time = {};
  std::chrono::nanoseconds origin_create_time = {};
  std::chrono::nanoseconds origin_create_time_utc = {};
};

}  // namespace roq

template <>
struct fmt::formatter<roq::TraceInfo> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::TraceInfo const &value, Context &context) {
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
