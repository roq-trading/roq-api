/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/compat.hpp"

namespace roq {
namespace server {

struct ROQ_PUBLIC TraceInfo final {
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
    return std::begin(context);
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
