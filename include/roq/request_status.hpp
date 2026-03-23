/* Copyright (c) 2017-2026, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cstdint>

namespace roq {

//! Enumeration of request status types
enum class RequestStatus : uint8_t {
  UNDEFINED = 0,
  FORWARDED,     //!< Forwarded to broker/exchange
  ACCEPTED,      //!< Accepted by broker/exchange
  REJECTED,      //!< Rejected by broker/exchange
  DISCONNECTED,  //!< Disconnected
  TIMEOUT,       //!< Timed out
  FAILED,        //!< Generic failure
  ERROR,         //!< Generic error, possibly parse error
  ENQUEUED,      //!< Enqueued for later release time to broker/exchange
};

}  // namespace roq
