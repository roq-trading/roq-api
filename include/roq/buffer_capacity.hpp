/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <magic_enum.hpp>

#include <fmt/core.h>

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
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::BufferCapacity const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};
