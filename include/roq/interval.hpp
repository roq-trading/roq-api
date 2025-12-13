/* Copyright (c) 2017-2026, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cstdint>

namespace roq {

//! Enumeration of time-series update frequencies
enum class Interval : uint8_t {
  UNDEFINED = 0,
  _1,     //!< 1 second
  _5,     //!< 5 seconds
  _10,    //!< 10 seconds
  _15,    //!< 15 seconds
  _30,    //!< 30 seconds
  _60,    //!< 1 minute
  _300,   //!< 5 minutes
  _600,   //!< 10 minutes
  _900,   //!< 15 minutes
  _1800,  //!< 30 minutes
};

}  // namespace roq
