/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/clock.hpp"
#include "roq/message_info.hpp"

namespace roq {

// note!
// certain use-cases are not allowed, e.g. zero initialization, copy construction and assignment
// this is done to prevent specific mistakes from happening

struct ROQ_PUBLIC TraceInfo final {
  // note! default initialization is using *current* time
  TraceInfo()
      : source_receive_time{clock::get_system()}, origin_create_time{source_receive_time},
        origin_create_time_utc{clock::get_realtime()} {}

  // note! meant for internal use, only
  TraceInfo(auto source_receive_time, auto origin_create_time, auto origin_create_time_utc)
      : source_receive_time{source_receive_time}, origin_create_time{origin_create_time},
        origin_create_time_utc{origin_create_time_utc} {}

  // note! meant for internal use, only
  TraceInfo(MessageInfo const &message_info)
      : TraceInfo{
            message_info.source_receive_time,
            message_info.origin_create_time,
            message_info.origin_create_time_utc,
        } {}

  TraceInfo(TraceInfo const &) = delete;
  void operator=(TraceInfo const &) = delete;

  std::chrono::nanoseconds const source_receive_time;
  std::chrono::nanoseconds const origin_create_time;
  std::chrono::nanoseconds const origin_create_time_utc;
};

}  // namespace roq

template <>
struct fmt::formatter<roq::TraceInfo> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::TraceInfo const &value, format_context &context) const {
    using namespace std::literals;
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
