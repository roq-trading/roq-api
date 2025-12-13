/* Copyright (c) 2017-2026, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cstdint>

namespace roq {

//! Enumeration of origin types
enum class Origin : uint8_t {
  UNDEFINED = 0,
  CLIENT,    //!< Client
  GATEWAY,   //!< Gateway
  BROKER,    //!< Broker
  EXCHANGE,  //!< Exchange
};

}  // namespace roq
