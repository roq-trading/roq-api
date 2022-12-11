/* Copyright (c) 2017-2023, Hans Erik Thrane */

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
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Version const &value, Context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}.{}.{}"sv, value.major, value.minor, value.revision);
  }
};
