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

//! Connected to gateway
struct ROQ_PUBLIC Connected final {};

template <>
inline constexpr std::string_view get_name<Connected>() {
  using namespace std::literals;
  return "connected"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::Connected> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Connected const &, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), R"({{}})"sv);
  }
};
