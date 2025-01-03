/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cstdint>

namespace roq {

//! Enumeration of security/instrument types
enum class SecurityType : uint8_t {
  UNDEFINED = 0,
  SPOT,
  FUTURES,
  OPTION,
  SWAP,  //!< Perpetuals (like a futures contract but without an expiration date)
};

}  // namespace roq
