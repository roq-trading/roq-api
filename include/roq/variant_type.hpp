/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/core.h>

#include <magic_enum/magic_enum_format.hpp>

namespace roq {

//! Enumeration of variant types
enum class VariantType : uint8_t {
  UNDEFINED = 0,
  STRING,    //!< ASCII string
  BOOL,      //!< Boolean
  INT8,      //!< Signed 8 bit integer
  UINT8,     //!< Unsigned 8 bit integer
  INT16,     //!< Signed 16 bit integer
  UINT16,    //!< Unsigned 16 bit integer
  INT32,     //!< Signed 32 bit integer
  UINT32,    //!< Unsigned 32 bit integer
  INT64,     //!< Signed 64 bit integer
  UINT64,    //!< Unsigned 64 bit integer
  FLOAT,     //!< 32 bit floating point
  DOUBLE,    //!< 64 bit floating point
  ENUM,      //!< Enumeration
  DATETIME,  //!< Nanoseconds relative to Unix epoch
};

}  // namespace roq
