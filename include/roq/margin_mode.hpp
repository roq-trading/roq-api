/* Copyright (c) 2017-2026, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cstdint>

namespace roq {

//! Enumeration of margin modes
enum class MarginMode : uint8_t {
  UNDEFINED = 0,
  ISOLATED,   //!< Isolated
  CROSS,      //!< Cross
  PORTFOLIO,  //!< Portfolio
};

}  // namespace roq
