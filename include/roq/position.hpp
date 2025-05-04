/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include <magic_enum/magic_enum_format.hpp>

#include "roq/limits.hpp"
#include "roq/string_types.hpp"

namespace roq {

//! Position
struct ROQ_PUBLIC Position final {
  roq::Exchange exchange;       //!< Exchange
  roq::Symbol symbol;           //!< Symbol
  double long_position = 0.0;   //!< Position (long)
  double short_position = 0.0;  //!< Position (short)
};

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
