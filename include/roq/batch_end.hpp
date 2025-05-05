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

//! Identifies the end of a sequence of related messages
struct ROQ_PUBLIC BatchEnd final {};

template <>
constexpr std::string_view get_name<BatchEnd>() {
  using namespace std::literals;
  return "batch_end"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::BatchEnd> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::BatchEnd const &, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), R"({{}})"sv);
  }
};
