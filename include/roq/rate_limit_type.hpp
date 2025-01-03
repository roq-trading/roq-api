/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cstdint>

namespace roq {

//! Rate limit type
enum class RateLimitType : uint8_t {
  UNDEFINED = 0,
  ORDER_ACTION,    //!< Any order action: create, modify, cancel, etc.
  CREATE_ORDER,    //!< Only create order
  REQUEST,         //!< Requests (count)
  REQUEST_WEIGHT,  //!< Requests (weighted)
};

}  // namespace roq
