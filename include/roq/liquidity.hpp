/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cstdint>

namespace roq {

//! Enumeration of liquidity type
enum class Liquidity : uint8_t {
  UNDEFINED = 0,
  MAKER,  //!< Maker
  TAKER,  //!< Taker
};

}  // namespace roq
