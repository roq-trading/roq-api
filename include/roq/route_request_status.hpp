/* Copyright (c) 2017-2026, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cstdint>

namespace roq {

//! Route request status
enum class RouteRequestStatus : uint8_t {
  UNDEFINED = 0,
  CREATED,   //!< Created
  REMOVED,   //!< Removed
  REJECTED,  //!< Rejected
};

}  // namespace roq
