/* Copyright (c) 2017-2026, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>
#include <fmt/ranges.h>

#include <magic_enum/magic_enum_format.hpp>

#include <span>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Add route(s)
struct ROQ_PUBLIC AddRoutes final {
  std::span<uint32_t const> strategy_ids;  //!< List of strategy_id's to add
};

template <>
constexpr std::string_view get_name<AddRoutes>() {
  using namespace std::literals;
  return "add_routes"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::AddRoutes> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::AddRoutes const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(strategy_ids=[{}])"
        R"(}})"sv,
        fmt::join(value.strategy_ids, ", "sv));
  }
};