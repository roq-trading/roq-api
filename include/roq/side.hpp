/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <magic_enum.hpp>

#include <fmt/format.h>

namespace roq {

//! Enumeration of the side of a trade
enum class Side : uint8_t {
  UNDEFINED = 0,
  BUY,
  SELL,
};

}  // namespace roq

template <>
struct fmt::formatter<roq::Side> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Side const &value, Context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};
