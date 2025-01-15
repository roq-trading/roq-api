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
