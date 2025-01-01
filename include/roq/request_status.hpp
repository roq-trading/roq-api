/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/core.h>

#include <magic_enum/magic_enum_format.hpp>

namespace roq {

//! Enumeration of request status types
enum class RequestStatus : uint8_t {
  UNDEFINED = 0,
  FORWARDED,
  ACCEPTED,
  REJECTED,
  DISCONNECTED,
  TIMEOUT,
  FAILED,  //!< Generic failure
  ERROR,   //!< Generic error, possibly parse error
};

}  // namespace roq
