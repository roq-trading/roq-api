/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/string_buffer.h"

namespace roq {

//! Rate-limit usage vs high-water mark
struct ROQ_PUBLIC RateLimitUsage final {
  uint16_t stream_id = {};             //!< Stream identifier
  bool above_high_water_mark = false;  //!< true when there is an increased probability that the next request could fail
};

}  // namespace roq

template <>
struct fmt::formatter<roq::RateLimitUsage> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::RateLimitUsage &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(above_high_water_mark={})"
        R"(}})"_sv,
        value.stream_id,
        value.above_high_water_mark);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::RateLimitUsage> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Event<roq::RateLimitUsage> &event, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(rate_limit_usage={})"
        R"(}})"_sv,
        event.message_info,
        event.value);
  }
};
