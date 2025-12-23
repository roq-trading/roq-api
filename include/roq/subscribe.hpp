/* Copyright (c) 2017-2026, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <magic_enum/magic_enum_format.hpp>

#include <span>
#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/string_types.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Dynamic subscription
struct ROQ_PUBLIC Subscribe final {
  std::string_view exchange;             //!< Exchange
  std::span<roq::Symbol const> symbols;  //!< Symbols
};

template <>
constexpr std::string_view get_name<Subscribe>() {
  using namespace std::literals;
  return "subscribe"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::Subscribe> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Subscribe const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(exchange="{}", )"
        R"(symbols=[{}])"
        R"(}})"sv,
        value.exchange,
        fmt::join(value.symbols, ", "sv));
  }
};