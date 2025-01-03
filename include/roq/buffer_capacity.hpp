/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cstdint>

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
