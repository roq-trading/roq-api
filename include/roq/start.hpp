/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include <magic_enum/magic_enum_format.hpp>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Signals a start event
struct ROQ_PUBLIC Start final {};

template <>
inline constexpr std::string_view get_name<Start>() {
  using namespace std::literals;
  return "start"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::Start> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Start const &, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), R"({{}})"sv);
  }
};
