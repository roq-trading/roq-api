/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include <limits>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/string_types.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Position
struct ROQ_PUBLIC Position final {
  roq::Exchange exchange;       //!< Exchange
  roq::Symbol symbol;           //!< Symbol
  double long_position = 0.0;   //!< Position (long)
  double short_position = 0.0;  //!< Position (short)
};

template <>
inline constexpr std::string_view get_name<Position>() {
  using namespace std::literals;
  return "position"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::Position> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Position const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(long_position={}, )"
        R"(short_position={})"
        R"(}})"sv,
        value.exchange,
        value.symbol,
        value.long_position,
        value.short_position);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::Position>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::Position> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(position={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::Position>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::Position> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(position={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
