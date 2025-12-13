/* Copyright (c) 2017-2026, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include <magic_enum/magic_enum_format.hpp>

#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Emitted when a new market is being created by the framework
struct ROQ_PUBLIC AddMarket final {
  std::string_view exchange;  //!< Exchange
  std::string_view symbol;    //!< Symbol
};

template <>
constexpr std::string_view get_name<AddMarket>() {
  using namespace std::literals;
  return "add_market"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::AddMarket> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::AddMarket const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(exchange="{}", )"
        R"(symbol="{}")"
        R"(}})"sv,
        value.exchange,
        value.symbol);
  }
};