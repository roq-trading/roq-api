/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>

#include "roq/compat.h"
#include "roq/literals.h"
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

  template <typename... Args>
  Exception(const source_location &loc, const fmt::format_string<Args...> &fmt, Args &&...args)
      : file_(basename(loc.file_name())), line_(loc.line()), what_(fmt::format(fmt, std::forward<Args>(args)...)) {}

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
  RuntimeErrorException(
      const fmt::format_string<Args...> &fmt, Args &&...args, const source_location &loc = source_location::current())
      : RuntimeError(loc, fmt, std::forward<Args>(args)...) {}
};

template <typename... Args>
RuntimeErrorException(const fmt::format_string<Args...> &, Args &&...) -> RuntimeErrorException<Args...>;

//! SystemError
class ROQ_PUBLIC SystemError : public RuntimeError {
 public:
  const std::error_code &code() const noexcept { return ec_; }

 protected:
  template <typename... Args>
  SystemError(const source_location &loc, std::error_code ec, const fmt::format_string<Args...> &fmt, Args &&...args)
      : RuntimeError(loc, fmt, std::forward<Args>(args)...), ec_(ec) {}

 private:
  const std::error_code ec_;
};

template <typename ErrorCode, typename... Args>
struct SystemErrorException final : public SystemError {
  SystemErrorException(
      ErrorCode ec,
      const fmt::format_string<Args...> &fmt,
      Args &&...args,
      const source_location &loc = source_location::current())
      : SystemError(loc, ec, fmt, std::forward<Args>(args)...) {}
};

template <typename ErrorCode, typename... Args>
SystemErrorException(ErrorCode, const fmt::format_string<Args...> &, Args &&...)
    -> SystemErrorException<ErrorCode, Args...>;

//! RangeError
class ROQ_PUBLIC RangeError : public RuntimeError {
 protected:
  using RuntimeError::RuntimeError;
};

template <typename... Args>
struct RangeErrorException final : public RangeError {
  RangeErrorException(
      const fmt::format_string<Args...> &fmt, Args &&...args, const source_location &loc = source_location::current())
      : RangeError(loc, fmt, std::forward<Args>(args)...) {}
};

template <typename... Args>
RangeErrorException(const fmt::format_string<Args...> &, Args &&...) -> RangeErrorException<Args...>;

//! OverflowError
class ROQ_PUBLIC OverflowError : public RuntimeError {
 protected:
  using RuntimeError::RuntimeError;
};

template <typename... Args>
struct OverflowErrorException final : public OverflowError {
  OverflowErrorException(
      const fmt::format_string<Args...> &fmt, Args &&...args, const source_location &loc = source_location::current())
      : OverflowError(loc, fmt, std::forward<Args>(args)...) {}
};

template <typename... Args>
OverflowErrorException(const fmt::format_string<Args...> &, Args &&...) -> OverflowErrorException<Args...>;

//! LogicError
class ROQ_PUBLIC LogicError : public Exception {
 protected:
  using Exception::Exception;
};

template <typename... Args>
struct LogicErrorException final : public LogicError {
  LogicErrorException(
      const fmt::format_string<Args...> &fmt, Args &&...args, const source_location &loc = source_location::current())
      : LogicError(loc, fmt, std::forward<Args>(args)...) {}
};

template <typename... Args>
LogicErrorException(const fmt::format_string<Args...> &, Args &&...) -> LogicErrorException<Args...>;

//! InvalidArgument
class ROQ_PUBLIC InvalidArgument : public LogicError {
 protected:
  using LogicError::LogicError;
};

template <typename... Args>
struct InvalidArgumentException final : public InvalidArgument {
  InvalidArgumentException(
      const fmt::format_string<Args...> &fmt, Args &&...args, const source_location &loc = source_location::current())
      : InvalidArgument(loc, fmt, std::forward<Args>(args)...) {}
};

template <typename... Args>
InvalidArgumentException(const fmt::format_string<Args...> &, Args &&...) -> InvalidArgumentException<Args...>;

//! OutOfRange
class ROQ_PUBLIC OutOfRange : public LogicError {
 protected:
  using LogicError::LogicError;
};

template <typename... Args>
struct OutOfRangeException final : public OutOfRange {
  OutOfRangeException(
      const fmt::format_string<Args...> &fmt, Args &&...args, const source_location &loc = source_location::current())
      : OutOfRange(loc, fmt, std::forward<Args>(args)...) {}
};

template <typename... Args>
OutOfRangeException(const fmt::format_string<Args...> &, Args &&...) -> OutOfRangeException<Args...>;

//! LengthError
class ROQ_PUBLIC LengthError : public LogicError {
 protected:
  using LogicError::LogicError;
};

template <typename... Args>
struct LengthErrorException final : public LengthError {
  LengthErrorException(
      const fmt::format_string<Args...> &fmt, Args &&...args, const source_location &loc = source_location::current())
      : LengthError(loc, fmt, std::forward<Args>(args)...) {}
};

template <typename... Args>
LengthErrorException(const fmt::format_string<Args...> &, Args &&...) -> LengthErrorException<Args...>;

// ROQ SPECIFIC

//! Fatal
class ROQ_PUBLIC Fatal : public RuntimeError {
 protected:
  using RuntimeError::RuntimeError;
};

template <typename... Args>
struct FatalException final : public Fatal {
  FatalException(
      const fmt::format_string<Args...> &fmt, Args &&...args, const source_location &loc = source_location::current())
      : Fatal(loc, fmt, std::forward<Args>(args)...) {}
};

template <typename... Args>
FatalException(const fmt::format_string<Args...> &, Args &&...) -> FatalException<Args...>;

//! File does not exist
class ROQ_PUBLIC FileDoesNotExist : public RuntimeError {
 protected:
  using RuntimeError::RuntimeError;
};

template <typename... Args>
struct FileDoesNotExistException final : public FileDoesNotExist {
  FileDoesNotExistException(
      const fmt::format_string<Args...> &fmt, Args &&...args, const source_location &loc = source_location::current())
      : FileDoesNotExist(loc, fmt, std::forward<Args>(args)...) {}
};

template <typename... Args>
FileDoesNotExistException(const fmt::format_string<Args...> &, Args &&...) -> FileDoesNotExistException<Args...>;

//! Not ready
class ROQ_PUBLIC NotReady : public RuntimeError {
 protected:
  using RuntimeError::RuntimeError;
};

template <typename... Args>
struct NotReadyException final : public NotReady {
  NotReadyException(
      const fmt::format_string<Args...> &fmt, Args &&...args, const source_location &loc = source_location::current())
      : NotReady(loc, fmt, std::forward<Args>(args)...) {}
};

template <typename... Args>
NotReadyException(const fmt::format_string<Args...> &, Args &&...) -> NotReadyException<Args...>;

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
  NotConnectedException(
      const fmt::format_string<Args...> &fmt, Args &&...args, const source_location &loc = source_location::current())
      : NotConnected(loc, fmt, std::forward<Args>(args)...) {}
};

template <typename... Args>
NotConnectedException(const fmt::format_string<Args...> &, Args &&...) -> NotConnectedException<Args...>;

//! Connection refused
class ROQ_PUBLIC ConnectionRefused : public TransportError {
 protected:
  using TransportError::TransportError;
};

template <typename... Args>
struct ConnectionRefusedException final : public ConnectionRefused {
  ConnectionRefusedException(
      const fmt::format_string<Args...> &fmt, Args &&...args, const source_location &loc = source_location::current())
      : ConnectionRefused(loc, fmt, std::forward<Args>(args)...) {}
};

template <typename... Args>
ConnectionRefusedException(const fmt::format_string<Args...> &, Args &&...) -> ConnectionRefusedException<Args...>;

//! Timed out
class ROQ_PUBLIC TimedOut : public TransportError {
 protected:
  using TransportError::TransportError;
};

template <typename... Args>
struct TimedOutException final : public TimedOut {
  TimedOutException(
      const fmt::format_string<Args...> &fmt, Args &&...args, const source_location &loc = source_location::current())
      : TimedOut(loc, fmt, std::forward<Args>(args)...) {}
};

template <typename... Args>
TimedOutException(const fmt::format_string<Args...> &, Args &&...) -> TimedOutException<Args...>;

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
  PermissionDeniedException(
      const fmt::format_string<Args...> &fmt, Args &&...args, const source_location &loc = source_location::current())
      : PermissionDenied(loc, fmt, std::forward<Args>(args)...) {}
};

template <typename... Args>
PermissionDeniedException(const fmt::format_string<Args...> &, Args &&...) -> PermissionDeniedException<Args...>;

//! Order not live
class ROQ_PUBLIC OrderNotLive : public SessionError {
 protected:
  using SessionError::SessionError;
};

template <typename... Args>
struct OrderNotLiveException final : public OrderNotLive {
  OrderNotLiveException(
      const fmt::format_string<Args...> &fmt, Args &&...args, const source_location &loc = source_location::current())
      : OrderNotLive(loc, fmt, std::forward<Args>(args)...) {}
};

template <typename... Args>
OrderNotLiveException(const fmt::format_string<Args...> &, Args &&...) -> OrderNotLiveException<Args...>;

}  // namespace roq

template <>
struct fmt::formatter<roq::Exception> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Exception &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(
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
struct fmt::formatter<roq::RuntimeError> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::RuntimeError &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::SystemError> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::SystemError &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::RangeError> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::RangeError &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::OverflowError> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::OverflowError &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::LogicError> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::LogicError &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::InvalidArgument> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::InvalidArgument &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::OutOfRange> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::OutOfRange &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::LengthError> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::LengthError &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::Fatal> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Fatal &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::FileDoesNotExist> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::FileDoesNotExist &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::NotReady> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::NotReady &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::NetworkError> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::NetworkError &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::TransportError> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::TransportError &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::NotConnected> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::NotConnected &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::ConnectionRefused> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::ConnectionRefused &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::TimedOut> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::TimedOut &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::SessionError> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::SessionError &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::PermissionDenied> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::PermissionDenied &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::OrderNotLive> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::OrderNotLive &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(context.out(), "{}"_sv, static_cast<const roq::Exception &>(value));
  }
};
