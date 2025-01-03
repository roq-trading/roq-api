/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cstdint>

namespace roq {

//! Enumeration of order cancel policies
enum class OrderCancelPolicy : uint8_t {
  UNDEFINED = 0,
  MANAGED_ORDERS,
  BY_ACCOUNT,
  BY_STRATEGY,
};

}  // namespace roq
