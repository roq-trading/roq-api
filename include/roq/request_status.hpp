/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cstdint>

namespace roq {

//! Enumeration of request status types
enum class RequestStatus : uint8_t {
  UNDEFINED = 0,
  FORWARDED,     //!< Forwarded to broker/exchange
  ACCEPTED,      //!< Accepted
  REJECTED,      //!< Rejected
  DISCONNECTED,  //!< Disconnected
  TIMEOUT,       //!< Timed out
  FAILED,        //!< Generic failure
  ERROR,         //!< Generic error, possibly parse error
};

}  // namespace roq
