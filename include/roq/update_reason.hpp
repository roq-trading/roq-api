/* Copyright (c) 2017-2026, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cstdint>

namespace roq {

//! Enumeration of update reasons
enum class UpdateReason : uint8_t {
  UNDEFINED = 0,
  CREATED,   //!< Created
  MODIFIED,  //!< Modified
  CANCELED,  //!< Canceled
  FILLED,    //!< Filled
};

}  // namespace roq
