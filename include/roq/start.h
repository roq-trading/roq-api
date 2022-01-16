/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/compat.h"
#include "roq/event.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/string_buffer.h"

namespace roq {

//! Signals a start event
struct ROQ_PUBLIC Start final {};

}  // namespace roq

template <>
struct fmt::formatter<roq::Start> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::Start &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(ctx.out(), R"({{}})"sv);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::Start> > {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::Event<roq::Start> &event, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(start={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
