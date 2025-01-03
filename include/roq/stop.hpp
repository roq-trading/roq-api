/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/core.h>

#include <magic_enum/magic_enum_format.hpp>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Signals a stop event
struct ROQ_PUBLIC Stop final {};

template <>
inline constexpr std::string_view get_name<Stop>() {
  using namespace std::literals;
  return "stop"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::Stop> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Stop const &, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), R"({{}})"sv);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::Stop>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::Stop> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stop={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::Stop>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::Stop> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stop={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
