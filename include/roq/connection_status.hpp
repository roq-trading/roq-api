/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cstdint>

namespace roq {

//! Enumeration of exchange connectivity status as seen from a gateway
enum class ConnectionStatus : uint8_t {
  UNDEFINED = 0,
  DISCONNECTED,
  CONNECTING,
  LOGIN_SENT,
  DOWNLOADING,
  READY,
  LOGGED_OUT,
};

}  // namespace roq
