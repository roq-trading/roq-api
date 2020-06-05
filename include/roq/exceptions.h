/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <stdexcept>
#include <string>
#include <string_view>

#include "roq/compat.h"

namespace roq {

class ROQ_PUBLIC Exception : public std::exception {
};

class ROQ_PUBLIC RuntimeError : public Exception {
 public:
  explicit RuntimeError(const std::string_view& what)
      : _what(what) {
  }
  const char *what() const noexcept override {
    return _what.c_str();
  }

 private:
  std::string _what;
};

class ROQ_PUBLIC Fatal final : public RuntimeError {
  using RuntimeError::RuntimeError;
};

class ROQ_PUBLIC FileDoesNotExist : public RuntimeError {
  using RuntimeError::RuntimeError;
};

class ROQ_PUBLIC NotReady : public RuntimeError {
  using RuntimeError::RuntimeError;
};

// network errors

class ROQ_PUBLIC NetworkError : public RuntimeError {
  using RuntimeError::RuntimeError;
};

// transport errors

class ROQ_PUBLIC TransportError : public NetworkError {
  using NetworkError::NetworkError;
};

class ROQ_PUBLIC NotConnected : public TransportError {
  using TransportError::TransportError;
};

class ROQ_PUBLIC ConnectionRefused : public TransportError {
  using TransportError::TransportError;
};

class ROQ_PUBLIC TimedOut : public TransportError {
  using TransportError::TransportError;
};

// session errors

class ROQ_PUBLIC SessionError : public NetworkError {
  using NetworkError::NetworkError;
};

class ROQ_PUBLIC PermissionDenied : public SessionError {
  using SessionError::SessionError;
};

class ROQ_PUBLIC OrderNotLive : public SessionError {
  using SessionError::SessionError;
};

}  // namespace roq
