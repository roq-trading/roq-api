/* Copyright (c) 2017-2026, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <magic_enum/magic_enum_format.hpp>

#include <chrono>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Represents a timer update
struct ROQ_PUBLIC Timer final {
  std::chrono::nanoseconds now = {};  //!< Current time (monotonic clock)
};

template <>
constexpr std::string_view get_name<Timer>() {
  using namespace std::literals;
  return "timer"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::Timer> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Timer const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(now={})"
        R"(}})"sv,
        value.now);
  }
};