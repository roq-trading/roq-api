/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <stdexcept>
#include <string>
#include <string_view>

#include "roq/compat.h"

namespace roq {

//! Base class for roq exceptions
class ROQ_PUBLIC Exception : public std::exception {};

//! Runtime error
class ROQ_PUBLIC RuntimeError : public Exception {
 public:
  explicit RuntimeError(const std::string_view &what) : what_(what) {}
  const char *what() const noexcept override { return what_.c_str(); }

 private:
  std::string what_;
};

//! Fatal
class ROQ_PUBLIC Fatal final : public RuntimeError {
  using RuntimeError::RuntimeError;
};

//! File does not exist
class ROQ_PUBLIC FileDoesNotExist : public RuntimeError {
  using RuntimeError::RuntimeError;
};

//! Not ready
class ROQ_PUBLIC NotReady : public RuntimeError {
  using RuntimeError::RuntimeError;
};

// network errors

//! Base class for network errors
class ROQ_PUBLIC NetworkError : public RuntimeError {
  using RuntimeError::RuntimeError;
};

// transport errors

//! Base class for transport errors
class ROQ_PUBLIC TransportError : public NetworkError {
  using NetworkError::NetworkError;
};

//! Not connected
class ROQ_PUBLIC NotConnected : public TransportError {
  using TransportError::TransportError;
};

//! Connection refused
class ROQ_PUBLIC ConnectionRefused : public TransportError {
  using TransportError::TransportError;
};

//! Timed out
class ROQ_PUBLIC TimedOut : public TransportError {
  using TransportError::TransportError;
};

// session errors

//! Base class for session errors
class ROQ_PUBLIC SessionError : public NetworkError {
  using NetworkError::NetworkError;
};

//! Permissions denied
class ROQ_PUBLIC PermissionDenied : public SessionError {
  using SessionError::SessionError;
};

//! Order not live
class ROQ_PUBLIC OrderNotLive : public SessionError {
  using SessionError::SessionError;
};

}  // namespace roq
