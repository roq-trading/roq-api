/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/core.h>

#include <magic_enum/magic_enum_format.hpp>

namespace roq {

//! Enumeration of protocol types (layer 7)
enum class Protocol : uint8_t {
  UNDEFINED = 0,
  FIX,   //!< FIX
  WS,    //!< Web-Socket
  HTTP,  //!< HTTP (REST)
  SBE,   //!< Simple Binary Encoding
  ROQ,   //!< UDP messaging
};

}  // namespace roq
