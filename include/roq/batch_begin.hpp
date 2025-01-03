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

//! Identifies the beginning of a sequence of related messages
struct ROQ_PUBLIC BatchBegin final {};

template <>
inline constexpr std::string_view get_name<BatchBegin>() {
  using namespace std::literals;
  return "batch_begin"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::BatchBegin> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::BatchBegin const &, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), R"({{}})"sv);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::BatchBegin>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::BatchBegin> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(batch_begin={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::BatchBegin>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::BatchBegin> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(batch_begin={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
