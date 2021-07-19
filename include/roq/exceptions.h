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

// This class hierarchy is *similar to* that of std::exception,
// but not trying to be a mirror!
// The reason is that a mirrored exception hierarchy would require multiple
// inheritance and then cause issues with catch handlers.
// The implication is that you must implement two exception handlers,
// e.g.
//   try {
//     ...
//   } catch (std::overflow_error&) {
//     ...
//   } catch (roq::OverflowError&) {
//     ...
//   }

//! Base
class ROQ_PUBLIC Exception : public std::exception {
 public:
  virtual const std::string_view file() const noexcept { return file_; }
  virtual int line() const noexcept { return line_; }

  char const *what() const noexcept override { return what_.c_str(); }

 protected:
  explicit Exception(const source_location &loc) : file_(basename(loc.file_name())), line_(loc.line()) {}
  Exception(const source_location &loc, const std::string &what)
      : file_(basename(loc.file_name())), line_(loc.line()), what_(what) {}

  static /*consteval*/ constexpr std::string_view basename(const std::string_view &path) noexcept {
    auto pos = path.find_last_of('/');
    return pos == path.npos ? path : path.substr(++pos);
  }

 private:
  const std::string_view file_;
  const int line_;
  const std::string what_;
};

//! Runtime error
class ROQ_PUBLIC RuntimeError : public Exception {
 protected:
  using Exception::Exception;
};

template <typename... Args>
struct RuntimeErrorException final : public RuntimeError {
  RuntimeErrorException(Args &&...args, const source_location &loc = source_location::current())
      : RuntimeError(loc, roq::format(std::forward<Args>(args)...)) {}
};

template <typename... Args>
RuntimeErrorException(Args &&...) -> RuntimeErrorException<Args...>;

//! SystemError
class ROQ_PUBLIC SystemError : public RuntimeError {
 public:
  const std::error_code &code() const noexcept { return ec_; }

 protected:
  SystemError(const source_location &loc, std::error_code ec, const std::string &what)
      : RuntimeError(loc, what), ec_(ec) {}

 private:
  const std::error_code ec_;
};

template <typename ErrorCode, typename... Args>
struct SystemErrorException final : public SystemError {
  SystemErrorException(ErrorCode ec, Args &&...args, const source_location &loc = source_location::current())
      : SystemError(loc, ec, roq::format(std::forward<Args>(args)...)) {}
};

template <typename ErrorCode, typename... Args>
SystemErrorException(ErrorCode, Args &&...) -> SystemErrorException<ErrorCode, Args...>;

//! RangeError
class ROQ_PUBLIC RangeError : public RuntimeError {
 protected:
  using RuntimeError::RuntimeError;
};

template <typename... Args>
struct RangeErrorException final : public RangeError {
  RangeErrorException(Args &&...args, const source_location &loc = source_location::current())
      : RangeError(loc, roq::format(std::forward<Args>(args)...)) {}
};

template <typename... Args>
RangeErrorException(Args &&...) -> RangeErrorException<Args...>;

//! OverflowError
class ROQ_PUBLIC OverflowError : public RuntimeError {
 protected:
  using RuntimeError::RuntimeError;
};

template <typename... Args>
struct OverflowErrorException final : public OverflowError {
  OverflowErrorException(Args &&...args, const source_location &loc = source_location::current())
      : OverflowError(loc, roq::format(std::forward<Args>(args)...)) {}
};

template <typename... Args>
OverflowErrorException(Args &&...) -> OverflowErrorException<Args...>;

//! LogicError
class ROQ_PUBLIC LogicError : public Exception {
 protected:
  using Exception::Exception;
};

template <typename... Args>
struct LogicErrorException final : public LogicError {
  LogicErrorException(Args &&...args, const source_location &loc = source_location::current())
      : LogicError(loc, roq::format(std::forward<Args>(args)...)) {}
};

template <typename... Args>
LogicErrorException(Args &&...) -> LogicErrorException<Args...>;

//! InvalidArgument
class ROQ_PUBLIC InvalidArgument : public LogicError {
 protected:
  using LogicError::LogicError;
};

template <typename... Args>
struct InvalidArgumentException final : public InvalidArgument {
  InvalidArgumentException(Args &&...args, const source_location &loc = source_location::current())
      : InvalidArgument(loc, roq::format(std::forward<Args>(args)...)) {}
};

template <typename... Args>
InvalidArgumentException(Args &&...) -> InvalidArgumentException<Args...>;

//! OutOfRange
class ROQ_PUBLIC OutOfRange : public LogicError {
 protected:
  using LogicError::LogicError;
};

template <typename... Args>
struct OutOfRangeException final : public OutOfRange {
  OutOfRangeException(Args &&...args, const source_location &loc = source_location::current())
      : OutOfRange(loc, roq::format(std::forward<Args>(args)...)) {}
};

template <typename... Args>
OutOfRangeException(Args &&...) -> OutOfRangeException<Args...>;

//! LengthError
class ROQ_PUBLIC LengthError : public LogicError {
 protected:
  using LogicError::LogicError;
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
 protected:
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
 protected:
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
 protected:
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
 protected:
  using RuntimeError::RuntimeError;
};

// transport errors

//! Base class for transport errors
class ROQ_PUBLIC TransportError : public NetworkError {
 protected:
  using NetworkError::NetworkError;
};

//! Not connected
class ROQ_PUBLIC NotConnected : public TransportError {
 protected:
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
 protected:
  using TransportError::TransportError;
};

template <typename... Args>
struct ConnectionRefusedException final : public ConnectionRefused {
  ConnectionRefusedException(Args &&...args, const source_location &loc = source_location::current())
      : ConnectionRefused(loc, roq::format(std::forward<Args>(args)...)) {}
};

template <typename... Args>
ConnectionRefusedException(Args &&...) -> ConnectionRefusedException<Args...>;

//! Timed out
class ROQ_PUBLIC TimedOut : public TransportError {
 protected:
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
 protected:
  using NetworkError::NetworkError;
};

//! Permissions denied
class ROQ_PUBLIC PermissionDenied : public SessionError {
 protected:
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
 protected:
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
        R"(type="{}", )"
        R"(what="{}", )"
        R"(file="{}", )"
        R"(line={})"
        R"(}})"_sv,
        typeid(value).name(),
        value.what(),
        value.file(),
        value.line());
  }
};

template <>
struct fmt::formatter<roq::RuntimeError> : public roq::formatter {
  template <typename Context>
  auto format(const roq::RuntimeError &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::SystemError> : public roq::formatter {
  template <typename Context>
  auto format(const roq::SystemError &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::RangeError> : public roq::formatter {
  template <typename Context>
  auto format(const roq::RangeError &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::OverflowError> : public roq::formatter {
  template <typename Context>
  auto format(const roq::OverflowError &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::LogicError> : public roq::formatter {
  template <typename Context>
  auto format(const roq::LogicError &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::InvalidArgument> : public roq::formatter {
  template <typename Context>
  auto format(const roq::InvalidArgument &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::OutOfRange> : public roq::formatter {
  template <typename Context>
  auto format(const roq::OutOfRange &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::LengthError> : public roq::formatter {
  template <typename Context>
  auto format(const roq::LengthError &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::Fatal> : public roq::formatter {
  template <typename Context>
  auto format(const roq::Fatal &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::FileDoesNotExist> : public roq::formatter {
  template <typename Context>
  auto format(const roq::FileDoesNotExist &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::NotReady> : public roq::formatter {
  template <typename Context>
  auto format(const roq::NotReady &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::NetworkError> : public roq::formatter {
  template <typename Context>
  auto format(const roq::NetworkError &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::TransportError> : public roq::formatter {
  template <typename Context>
  auto format(const roq::TransportError &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::NotConnected> : public roq::formatter {
  template <typename Context>
  auto format(const roq::NotConnected &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::ConnectionRefused> : public roq::formatter {
  template <typename Context>
  auto format(const roq::ConnectionRefused &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::TimedOut> : public roq::formatter {
  template <typename Context>
  auto format(const roq::TimedOut &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::SessionError> : public roq::formatter {
  template <typename Context>
  auto format(const roq::SessionError &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::PermissionDenied> : public roq::formatter {
  template <typename Context>
  auto format(const roq::PermissionDenied &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::OrderNotLive> : public roq::formatter {
  template <typename Context>
  auto format(const roq::OrderNotLive &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};
