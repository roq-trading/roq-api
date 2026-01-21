/* Copyright (c) 2017-2026, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cstdint>

namespace roq {

//! Request identifier
enum class RequestIdType : uint8_t {
  UNDEFINED = 0,
  BASE64,  //!< Base64 encoded
  BASE32,  //!< Base32 encoded
  SIMPLE,  //!< Simple encoded
  UUID,    //!< UUID encoded
  HEX,     //!< Hex encoded
};

}  // namespace roq
