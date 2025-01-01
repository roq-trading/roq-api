/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/core.h>

#include <magic_enum/magic_enum_format.hpp>

namespace roq {

//! Route request status
enum class RouteRequestStatus : uint8_t {
  UNDEFINED = 0,
  CREATED,   //!< Created
  REMOVED,   //!< Removed
  REJECTED,  //!< Rejected
};

}  // namespace roq
