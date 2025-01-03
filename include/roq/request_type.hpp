/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cstdint>

namespace roq {

//! Enumeration of request types
enum class RequestType : uint8_t {
  UNDEFINED = 0,
  CREATE_ORDER,
  MODIFY_ORDER,
  CANCEL_ORDER,
};

}  // namespace roq
