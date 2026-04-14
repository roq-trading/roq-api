/* Copyright (c) 2017-2026, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cstdint>

namespace roq {

//! Rate limit type
enum class RateLimitType : uint8_t {
  UNDEFINED = 0,
  ORDER_ACTION,             //!< Any order action: create, modify, cancel, etc.
  CREATE_ORDER,             //!< Only create order
  REQUEST,                  //!< Requests (count)
  REQUEST_WEIGHT,           //!< Requests (weighted)
  CREATE_AND_MODIFY_ORDER,  //!< Includes create and modify order requests
  MODIFY_ORDER,             //!< Modify order requests
  CANCEL_ORDER,             //!< Cancel order requests
};

}  // namespace roq
