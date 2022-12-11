/* Copyright (c) 2017-2023, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <magic_enum.hpp>

#include <fmt/format.h>

namespace roq {

//! Buffer capacity
enum class BufferCapacity : uint8_t {
  UNDEFINED = 0,
  EMPTY,            //!< Buffer is empty
  LOW_WATER_MARK,   //!< Buffer has reached the low water mark (from above)
  HIGH_WATER_MARK,  //!< Buffer has reached the high water mark (from below)
  FULL,             //!< Buffer is full
};

}  // namespace roq

template <>
struct fmt::formatter<roq::BufferCapacity> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::BufferCapacity const &value, Context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};
