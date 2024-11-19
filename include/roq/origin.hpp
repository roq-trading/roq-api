/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <magic_enum/magic_enum.hpp>

#include <fmt/core.h>

namespace roq {

//! Enumeration of origin types
enum class Origin : uint8_t {
  UNDEFINED = 0,
  CLIENT,
  GATEWAY,
  BROKER,
  EXCHANGE,
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Origin> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Origin const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};
