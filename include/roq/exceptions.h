/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>

#include "roq/compat.h"
#include "roq/format.h"
#include "roq/source_location.h"

namespace roq {

//! Base class for roq exceptions
class ROQ_PUBLIC Exception : public std::exception {
 public:
  virtual const std::string_view file() const noexcept = 0;
  virtual int line() const noexcept = 0;
};

//! Runtime error
class ROQ_PUBLIC RuntimeError : public Exception, public std::runtime_error {
 public:
  char const *what() const noexcept override { return std::runtime_error::what(); }
  const std::string_view file() const noexcept override { return file_; }
  int line() const noexcept override { return line_; }

 protected:
  template <typename... Args>
  RuntimeError(const source_location &loc, const std::string &what)
      : std::runtime_error(what), file_(basename(loc.file_name())), line_(loc.line()) {}

  static /*consteval*/ constexpr std::string_view basename(const std::string_view &path) noexcept {
    auto pos = path.find_last_of('/');
    return pos == path.npos ? path : path.substr(++pos);
  }

 private:
  const std::string_view file_;
  const int line_;
};

template <typename... Args>
struct RuntimeErrorException final : public RuntimeError {
  RuntimeErrorException(Args &&...args, const source_location &loc = source_location::current())
      : RuntimeError(loc, roq::format(std::forward<Args>(args)...)) {}
};

template <typename... Args>
RuntimeErrorException(Args &&...) -> RuntimeErrorException<Args...>;

//! SystemError
class ROQ_PUBLIC SystemError : public RuntimeError, public std::system_error {
 protected:
  SystemError(const source_location &loc, std::error_code ec, const std::string &what)
      : RuntimeError(loc, what), std::system_error(ec, what) {}
};

template <typename ErrorCode, typename... Args>
struct SystemErrorException final : public SystemError {
  SystemErrorException(
      ErrorCode ec, Args &&...args, const source_location &loc = source_location::current())
      : SystemError(loc, ec, roq::format(std::forward<Args>(args)...)) {}
};

template <typename ErrorCode, typename... Args>
SystemErrorException(ErrorCode, Args &&...) -> SystemErrorException<ErrorCode, Args...>;

//! RangeError
class ROQ_PUBLIC RangeError : public RuntimeError, public std::range_error {
 protected:
  RangeError(const source_location &loc, const std::string &what)
      : RuntimeError(loc, what), std::range_error(what) {}
};

template <typename... Args>
struct RangeErrorException final : public RangeError {
  RangeErrorException(Args &&...args, const source_location &loc = source_location::current())
      : RangeError(loc, roq::format(std::forward<Args>(args)...)) {}
};

template <typename... Args>
RangeErrorException(Args &&...) -> RangeErrorException<Args...>;

//! OverflowError
class ROQ_PUBLIC OverflowError : public RuntimeError, public std::overflow_error {
 protected:
  OverflowError(const source_location &loc, const std::string &what)
      : RuntimeError(loc, what), std::overflow_error(what) {}
};

template <typename... Args>
struct OverflowErrorException final : public OverflowError {
  OverflowErrorException(Args &&...args, const source_location &loc = source_location::current())
      : OverflowError(loc, roq::format(std::forward<Args>(args)...)) {}
};

template <typename... Args>
OverflowErrorException(Args &&...) -> OverflowErrorException<Args...>;

//! LogicError
class ROQ_PUBLIC LogicError : public Exception, public std::logic_error {
 public:
  char const *what() const noexcept override { return std::logic_error::what(); }
  const std::string_view file() const noexcept override { return file_; }
  int line() const noexcept override { return line_; }

 protected:
  template <typename... Args>
  LogicError(const source_location &loc, const std::string &what)
      : std::logic_error(what), file_(basename(loc.file_name())), line_(loc.line()) {}

  static /*consteval*/ constexpr std::string_view basename(const std::string_view &path) noexcept {
    auto pos = path.find_last_of('/');
    return pos == path.npos ? path : path.substr(++pos);
  }

 private:
  const std::string_view file_;
  const int line_;
};

template <typename... Args>
struct LogicErrorException final : public LogicError {
  LogicErrorException(Args &&...args, const source_location &loc = source_location::current())
      : LogicError(loc, roq::format(std::forward<Args>(args)...)) {}
};

template <typename... Args>
LogicErrorException(Args &&...) -> LogicErrorException<Args...>;

//! InvalidArgument
class ROQ_PUBLIC InvalidArgument : public LogicError, public std::invalid_argument {
 protected:
  InvalidArgument(const source_location &loc, const std::string &what)
      : LogicError(loc, what), std::invalid_argument(what) {}
};

template <typename... Args>
struct InvalidArgumentException final : public InvalidArgument {
  InvalidArgumentException(Args &&...args, const source_location &loc = source_location::current())
      : InvalidArgument(loc, roq::format(std::forward<Args>(args)...)) {}
};

template <typename... Args>
InvalidArgumentException(Args &&...) -> InvalidArgumentException<Args...>;

//! OutOfRange
class ROQ_PUBLIC OutOfRange : public LogicError, public std::out_of_range {
 protected:
  OutOfRange(const source_location &loc, const std::string &what)
      : LogicError(loc, what), std::out_of_range(what) {}
};

template <typename... Args>
struct OutOfRangeException final : public OutOfRange {
  OutOfRangeException(Args &&...args, const source_location &loc = source_location::current())
      : OutOfRange(loc, roq::format(std::forward<Args>(args)...)) {}
};

template <typename... Args>
OutOfRangeException(Args &&...) -> OutOfRangeException<Args...>;

//! LengthError
class ROQ_PUBLIC LengthError : public LogicError, public std::length_error {
 protected:
  LengthError(const source_location &loc, const std::string &what)
      : LogicError(loc, what), std::length_error(what) {}
};

template <typename... Args>
struct LengthErrorException final : public LengthError {
  LengthErrorException(Args &&...args, const source_location &loc = source_location::current())
      : LengthError(loc, roq::format(std::forward<Args>(args)...)) {}
};

template <typename... Args>
LengthErrorException(Args &&...) -> LengthErrorException<Args...>;

// ROQ SPECIFIC

//! Fatal
class ROQ_PUBLIC Fatal : public RuntimeError {
  using RuntimeError::RuntimeError;
};

template <typename... Args>
struct FatalException final : public Fatal {
  FatalException(Args &&...args, const source_location &loc = source_location::current())
      : Fatal(loc, roq::format(std::forward<Args>(args)...)) {}
};

template <typename... Args>
FatalException(Args &&...) -> FatalException<Args...>;

//! File does not exist
class ROQ_PUBLIC FileDoesNotExist : public RuntimeError {
  using RuntimeError::RuntimeError;
};

template <typename... Args>
struct FileDoesNotExistException final : public FileDoesNotExist {
  FileDoesNotExistException(Args &&...args, const source_location &loc = source_location::current())
      : FileDoesNotExist(loc, roq::format(std::forward<Args>(args)...)) {}
};

template <typename... Args>
FileDoesNotExistException(Args &&...) -> FileDoesNotExistException<Args...>;

//! Not ready
class ROQ_PUBLIC NotReady : public RuntimeError {
  using RuntimeError::RuntimeError;
};

template <typename... Args>
struct NotReadyException final : public NotReady {
  NotReadyException(Args &&...args, const source_location &loc = source_location::current())
      : NotReady(loc, roq::format(std::forward<Args>(args)...)) {}
};

template <typename... Args>
NotReadyException(Args &&...) -> NotReadyException<Args...>;

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

template <typename... Args>
struct NotConnectedException final : public NotConnected {
  NotConnectedException(Args &&...args, const source_location &loc = source_location::current())
      : NotConnected(loc, roq::format(std::forward<Args>(args)...)) {}
};

template <typename... Args>
NotConnectedException(Args &&...) -> NotConnectedException<Args...>;

//! Connection refused
class ROQ_PUBLIC ConnectionRefused : public TransportError {
  using TransportError::TransportError;
};

template <typename... Args>
struct ConnectionRefusedException final : public ConnectionRefused {
  ConnectionRefusedException(
      Args &&...args, const source_location &loc = source_location::current())
      : ConnectionRefused(loc, roq::format(std::forward<Args>(args)...)) {}
};

template <typename... Args>
ConnectionRefusedException(Args &&...) -> ConnectionRefusedException<Args...>;

//! Timed out
class ROQ_PUBLIC TimedOut : public TransportError {
  using TransportError::TransportError;
};

template <typename... Args>
struct TimedOutException final : public TimedOut {
  TimedOutException(Args &&...args, const source_location &loc = source_location::current())
      : TimedOut(loc, roq::format(std::forward<Args>(args)...)) {}
};

template <typename... Args>
TimedOutException(Args &&...) -> TimedOutException<Args...>;

// session errors

//! Base class for session errors
class ROQ_PUBLIC SessionError : public NetworkError {
  using NetworkError::NetworkError;
};

//! Permissions denied
class ROQ_PUBLIC PermissionDenied : public SessionError {
  using SessionError::SessionError;
};

template <typename... Args>
struct PermissionDeniedException final : public PermissionDenied {
  PermissionDeniedException(Args &&...args, const source_location &loc = source_location::current())
      : PermissionDenied(loc, roq::format(std::forward<Args>(args)...)) {}
};

template <typename... Args>
PermissionDeniedException(Args &&...) -> PermissionDeniedException<Args...>;

//! Order not live
class ROQ_PUBLIC OrderNotLive : public SessionError {
  using SessionError::SessionError;
};

template <typename... Args>
struct OrderNotLiveException final : public OrderNotLive {
  OrderNotLiveException(Args &&...args, const source_location &loc = source_location::current())
      : OrderNotLive(loc, roq::format(std::forward<Args>(args)...)) {}
};

template <typename... Args>
OrderNotLiveException(Args &&...) -> OrderNotLiveException<Args...>;

}  // namespace roq

template <>
struct fmt::formatter<roq::Exception> : public roq::formatter {
  template <typename Context>
  auto format(const roq::Exception &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(what="{}", )"
        R"(file="{}", )"
        R"(line={})"
        R"(}})"_fmt,
        value.what(),
        value.file(),
        value.line());
  }
};
