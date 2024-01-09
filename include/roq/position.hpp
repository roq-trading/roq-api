/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/compile.h>
#include <fmt/format.h>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/string_types.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Position
struct ROQ_PUBLIC Position final {
  Exchange exchange;            //!< Exchange
  Symbol symbol;                //!< Symbol
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
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Position const &value, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(long_position={}, )"
        R"(short_position={})"
        R"(}})"_cf,
        value.exchange,
        value.symbol,
        value.long_position,
        value.short_position);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::Position>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Event<roq::Position> const &event, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(position={}, )"
        R"(message_info={})"
        R"(}})"_cf,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::Position>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Trace<roq::Position> const &event, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(position={}, )"
        R"(trace_info={})"
        R"(}})"_cf,
        event.value,
        event.trace_info);
  }
};
