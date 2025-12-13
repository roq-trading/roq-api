/* Copyright (c) 2017-2026, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cstdint>

namespace roq {

//! Enumeration of order cancel policies
enum class OrderCancelPolicy : uint8_t {
  UNDEFINED = 0,
  MANAGED_ORDERS,  //!< Cancel managed orders
  BY_ACCOUNT,      //!< Cancel by account
  BY_STRATEGY,     //!< Cancel by strategy
};

}  // namespace roq
