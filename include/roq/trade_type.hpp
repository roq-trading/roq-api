/* Copyright (c) 2017-2026, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cstdint>

namespace roq {

//! Enumeration of the type of trade
enum class TradeType : uint8_t {
  UNDEFINED = 0,
  REGULAR,  //!< Regular trade
  BLOCK,    //!< Block trade
};

}  // namespace roq
