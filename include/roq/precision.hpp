/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <magic_enum.hpp>

#include <fmt/format.h>

namespace roq {

//! Enumeration of decimal precision (digits)
enum class Precision : uint8_t {
  UNDEFINED = 0,
  _0,
  _1,
  _2,
  _3,
  _4,
  _5,
  _6,
  _7,
  _8,
  _9,
  _10,
  _11,
  _12,
  _13,
  _14,
  _15,
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Precision> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Precision const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};
