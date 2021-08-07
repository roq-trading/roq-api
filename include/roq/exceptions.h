/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>

#include "roq/compat.h"
#include "roq/format_str.h"
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

struct RuntimeErrorException final : public RuntimeError {
  template <typename... Args>
  RuntimeErrorException(const format_str &fmt, Args &&...args)
      : RuntimeError(
            static_cast<const source_location &>(fmt),
            fmt::format_string<Args...>(static_cast<const std::string_view &>(fmt)),
            std::forward<Args>(args)...) {}
};

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

struct SystemErrorException final : public SystemError {
  template <typename ErrorCode, typename... Args>
  SystemErrorException(ErrorCode ec, const format_str &fmt, Args &&...args)
      : SystemError(
            static_cast<const source_location &>(fmt),
            ec,
            fmt::format_string<Args...>(static_cast<const std::string_view &>(fmt)),
            std::forward<Args>(args)...) {}
};

//! RangeError
class ROQ_PUBLIC RangeError : public RuntimeError {
 protected:
  using RuntimeError::RuntimeError;
};

struct RangeErrorException final : public RangeError {
  template <typename... Args>
  RangeErrorException(const format_str &fmt, Args &&...args)
      : RangeError(
            static_cast<const source_location &>(fmt),
            fmt::format_string<Args...>(static_cast<const std::string_view &>(fmt)),
            std::forward<Args>(args)...) {}
};

//! OverflowError
class ROQ_PUBLIC OverflowError : public RuntimeError {
 protected:
  using RuntimeError::RuntimeError;
};

struct OverflowErrorException final : public OverflowError {
  template <typename... Args>
  OverflowErrorException(const format_str &fmt, Args &&...args)
      : OverflowError(
            static_cast<const source_location &>(fmt),
            fmt::format_string<Args...>(static_cast<const std::string_view &>(fmt)),
            std::forward<Args>(args)...) {}
};

//! LogicError
class ROQ_PUBLIC LogicError : public Exception {
 protected:
  using Exception::Exception;
};

struct LogicErrorException final : public LogicError {
  template <typename... Args>
  LogicErrorException(const format_str &fmt, Args &&...args)
      : LogicError(
            static_cast<const source_location &>(fmt),
            fmt::format_string<Args...>(static_cast<const std::string_view &>(fmt)),
            std::forward<Args>(args)...) {}
};

//! InvalidArgument
class ROQ_PUBLIC InvalidArgument : public LogicError {
 protected:
  using LogicError::LogicError;
};

struct InvalidArgumentException final : public InvalidArgument {
  template <typename... Args>
  InvalidArgumentException(const format_str &fmt, Args &&...args)
      : InvalidArgument(
            static_cast<const source_location &>(fmt),
            fmt::format_string<Args...>(static_cast<const std::string_view &>(fmt)),
            std::forward<Args>(args)...) {}
};

//! OutOfRange
class ROQ_PUBLIC OutOfRange : public LogicError {
 protected:
  using LogicError::LogicError;
};

struct OutOfRangeException final : public OutOfRange {
  template <typename... Args>
  OutOfRangeException(const format_str &fmt, Args &&...args)
      : OutOfRange(
            static_cast<const source_location &>(fmt),
            fmt::format_string<Args...>(static_cast<const std::string_view &>(fmt)),
            std::forward<Args>(args)...) {}
};

//! LengthError
class ROQ_PUBLIC LengthError : public LogicError {
 protected:
  using LogicError::LogicError;
};

struct LengthErrorException final : public LengthError {
  template <typename... Args>
  LengthErrorException(const format_str &fmt, Args &&...args)
      : LengthError(
            static_cast<const source_location &>(fmt),
            fmt::format_string<Args...>(static_cast<const std::string_view &>(fmt)),
            std::forward<Args>(args)...) {}
};

// ROQ SPECIFIC

//! Fatal
class ROQ_PUBLIC Fatal : public RuntimeError {
 protected:
  using RuntimeError::RuntimeError;
};

struct FatalException final : public Fatal {
  template <typename... Args>
  FatalException(const format_str &fmt, Args &&...args)
      : Fatal(
            static_cast<const source_location &>(fmt),
            fmt::format_string<Args...>(static_cast<const std::string_view &>(fmt)),
            std::forward<Args>(args)...) {}
};

//! File does not exist
class ROQ_PUBLIC FileDoesNotExist : public RuntimeError {
 protected:
  using RuntimeError::RuntimeError;
};

struct FileDoesNotExistException final : public FileDoesNotExist {
  template <typename... Args>
  FileDoesNotExistException(const format_str &fmt, Args &&...args)
      : FileDoesNotExist(
            static_cast<const source_location &>(fmt),
            fmt::format_string<Args...>(static_cast<const std::string_view &>(fmt)),
            std::forward<Args>(args)...) {}
};

//! Not ready
class ROQ_PUBLIC NotReady : public RuntimeError {
 protected:
  using RuntimeError::RuntimeError;
};

struct NotReadyException final : public NotReady {
  template <typename... Args>
  NotReadyException(const format_str &fmt, Args &&...args)
      : NotReady(
            static_cast<const source_location &>(fmt),
            fmt::format_string<Args...>(static_cast<const std::string_view &>(fmt)),
            std::forward<Args>(args)...) {}
};

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

struct NotConnectedException final : public NotConnected {
  template <typename... Args>
  NotConnectedException(const format_str &fmt, Args &&...args)
      : NotConnected(
            static_cast<const source_location &>(fmt),
            fmt::format_string<Args...>(static_cast<const std::string_view &>(fmt)),
            std::forward<Args>(args)...) {}
};

//! Connection refused
class ROQ_PUBLIC ConnectionRefused : public TransportError {
 protected:
  using TransportError::TransportError;
};

struct ConnectionRefusedException final : public ConnectionRefused {
  template <typename... Args>
  ConnectionRefusedException(const format_str &fmt, Args &&...args)
      : ConnectionRefused(
            static_cast<const source_location &>(fmt),
            fmt::format_string<Args...>(static_cast<const std::string_view &>(fmt)),
            std::forward<Args>(args)...) {}
};

//! Timed out
class ROQ_PUBLIC TimedOut : public TransportError {
 protected:
  using TransportError::TransportError;
};

struct TimedOutException final : public TimedOut {
  template <typename... Args>
  TimedOutException(const format_str &fmt, Args &&...args)
      : TimedOut(
            static_cast<const source_location &>(fmt),
            fmt::format_string<Args...>(static_cast<const std::string_view &>(fmt)),
            std::forward<Args>(args)...) {}
};

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

struct PermissionDeniedException final : public PermissionDenied {
  template <typename... Args>
  PermissionDeniedException(const format_str &fmt, Args &&...args)
      : PermissionDenied(
            static_cast<const source_location &>(fmt),
            fmt::format_string<Args...>(static_cast<const std::string_view &>(fmt)),
            std::forward<Args>(args)...) {}
};

//! Order not live
class ROQ_PUBLIC OrderNotLive : public SessionError {
 protected:
  using SessionError::SessionError;
};

struct OrderNotLiveException final : public OrderNotLive {
  template <typename... Args>
  OrderNotLiveException(const format_str &fmt, Args &&...args)
      : OrderNotLive(
            static_cast<const source_location &>(fmt),
            fmt::format_string<Args...>(static_cast<const std::string_view &>(fmt)),
            std::forward<Args>(args)...) {}
};

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
