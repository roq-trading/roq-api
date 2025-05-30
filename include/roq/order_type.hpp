/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cstdint>

namespace roq {

//! Enumeration of order types
enum class OrderType : uint8_t {
  UNDEFINED = 0,
  MARKET,  //!< Market order
  LIMIT,   //!< Limit order
};

}  // namespace roq
