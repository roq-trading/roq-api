/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

namespace roq {

struct Version final {
  uint8_t major = {};
  uint8_t minor = {};
  uint16_t revision = {};
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Version> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Version const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}.{}.{}"sv, value.major, value.minor, value.revision);
  }
};
