/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/compat.hpp"
#include "roq/event.hpp"
#include "roq/message_info.hpp"
#include "roq/numbers.hpp"
#include "roq/string_buffer.hpp"

namespace roq {

//! Connected to gateway
struct ROQ_PUBLIC Connected final {};

}  // namespace roq

template <>
struct fmt::formatter<roq::Connected> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::Connected &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(ctx.out(), R"({{}})"sv);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::Connected> > {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::Event<roq::Connected> &event, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(connected={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
