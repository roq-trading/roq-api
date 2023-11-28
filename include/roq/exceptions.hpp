/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/compat.hpp"

#include <fmt/compile.h>
#include <fmt/format.h>

#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>

#include "roq/error.hpp"
#include "roq/format_str.hpp"
#include "roq/request_status.hpp"

namespace roq {

namespace detail {
template <typename... Args>
constexpr auto create_what(format_str<Args...> const &fmt, Args &&...args) -> std::string {
  if (std::size(fmt.str) == 0)
    return {};
  if constexpr (sizeof...(args) == 0) {
    return {std::data(fmt.str), std::size(fmt.str)};
  } else {
    return fmt::vformat(fmt.str, fmt::make_format_args(args...));
  }
}
}  // namespace detail

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
struct ROQ_PUBLIC Exception : public std::exception {
  template <typename... Args>
  explicit Exception(format_str<Args...> const &fmt, Args &&...args)
      : file_name_{fmt.file_name}, line_{fmt.line}, what_{detail::create_what(fmt, std::forward<Args>(args)...)} {}

  char const *what() const noexcept override { return what_.c_str(); }

  virtual std::string_view const file() const noexcept { return file_name_; }
  virtual int line() const noexcept { return line_; }

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(type="{}", )"
        R"(what="{}", )"
        R"(file="{}", )"
        R"(line={})"
        R"(}})"_cf,
        typeid(*this).name(),
        what_,
        file_name_,
        line_);
  }

 protected:
  detail::static_string<32> const file_name_;
  uint32_t const line_;
  std::string const what_;
};

//! Runtime error
struct ROQ_PUBLIC RuntimeError : public Exception {
  using Exception::Exception;
};

//! SystemError
struct ROQ_PUBLIC SystemError : public RuntimeError {
 public:
  std::error_code const &code() const noexcept { return ec_; }

  template <typename... Args>
  SystemError(std::error_code ec, format_str<Args...> const &fmt, Args &&...args)
      : RuntimeError{fmt, std::forward<Args>(args)...}, ec_{ec} {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(type="{}", )"
        R"(what="{}", )"
        R"(file="{}", )"
        R"(line={}, )"
        R"(error_code={{)"
        R"(message="{}", )"
        R"(value={})"
        R"(}})"
        R"(}})"_cf,
        typeid(*this).name(),
        what_,
        file_name_,
        line_,
        ec_.message(),
        ec_.value());
  }

 private:
  std::error_code const ec_;
};

//! RangeError
struct ROQ_PUBLIC RangeError : public RuntimeError {
  using RuntimeError::RuntimeError;
};

//! OverflowError
struct ROQ_PUBLIC OverflowError : public RuntimeError {
  using RuntimeError::RuntimeError;
};

//! LogicError
struct ROQ_PUBLIC LogicError : public Exception {
  using Exception::Exception;
};

//! InvalidArgument
struct ROQ_PUBLIC InvalidArgument : public LogicError {
  using LogicError::LogicError;
};

//! OutOfRange
struct ROQ_PUBLIC OutOfRange : public LogicError {
  using LogicError::LogicError;
};

//! LengthError
struct ROQ_PUBLIC LengthError : public LogicError {
  using LogicError::LogicError;
};

// ROQ SPECIFIC

//! Fatal
struct ROQ_PUBLIC Fatal : public RuntimeError {
  using RuntimeError::RuntimeError;
};

//! File does not exist
struct ROQ_PUBLIC FileDoesNotExist : public RuntimeError {
  using RuntimeError::RuntimeError;
};

//! Not ready
struct ROQ_PUBLIC NotReady : public RuntimeError {
  using RuntimeError::RuntimeError;
};

//! Not implemented
struct ROQ_PUBLIC NotImplemented : public RuntimeError {
  using RuntimeError::RuntimeError;
};

//! Not supported
struct ROQ_PUBLIC NotSupported : public RuntimeError {
  using RuntimeError::RuntimeError;
};

//! Bad state
struct ROQ_PUBLIC BadState : public RuntimeError {
  using RuntimeError::RuntimeError;
};

// network errors

//! Base class for network errors
struct ROQ_PUBLIC NetworkError : public RuntimeError {
  template <typename... Args>
  NetworkError(RequestStatus request_status, Error error, format_str<Args...> const &fmt, Args &&...args)
      : RuntimeError{fmt, std::forward<Args>(args)...}, request_status_{request_status}, error_{error} {}

  RequestStatus request_status() const noexcept { return request_status_; }
  Error error() const noexcept { return error_; }

 private:
  RequestStatus const request_status_;
  Error const error_;
};

// transport errors

//! Base class for transport errors
struct ROQ_PUBLIC TransportError : public NetworkError {
  using NetworkError::NetworkError;
};

//! Not connected
struct ROQ_PUBLIC NotConnected : public TransportError {
  template <typename... Args>
  explicit NotConnected(format_str<Args...> const &fmt, Args &&...args)
      : TransportError{RequestStatus::REJECTED, Error::GATEWAY_NOT_READY, fmt, std::forward<Args>(args)...} {}
};

//! Connection refused
struct ROQ_PUBLIC ConnectionRefused : public TransportError {
  template <typename... Args>
  explicit ConnectionRefused(format_str<Args...> const &fmt, Args &&...args)
      : TransportError{RequestStatus::REJECTED, Error::GATEWAY_NOT_READY, fmt, std::forward<Args>(args)...} {}
};

//! Timed out
struct ROQ_PUBLIC TimedOut : public TransportError {
  template <typename... Args>
  explicit TimedOut(format_str<Args...> const &fmt, Args &&...args)
      : TransportError{RequestStatus::TIMEOUT, Error::TIMEOUT, fmt, std::forward<Args>(args)...} {}
};

// session errors

//! Base class for session errors
struct ROQ_PUBLIC SessionError : public NetworkError {
  using NetworkError::NetworkError;
};

//! Permissions denied (operating system)
struct ROQ_PUBLIC PermissionDenied : public SessionError {
  template <typename... Args>
  explicit PermissionDenied(format_str<Args...> const &fmt, Args &&...args)
      : SessionError{RequestStatus::UNDEFINED, Error::UNDEFINED, fmt, std::forward<Args>(args)...} {}
};

//! Order not live
struct ROQ_PUBLIC OrderNotLive : public SessionError {
  template <typename... Args>
  explicit OrderNotLive(format_str<Args...> const &fmt, Args &&...args)
      : SessionError{RequestStatus::REJECTED, Error::TOO_LATE_TO_MODIFY_OR_CANCEL, fmt, std::forward<Args>(args)...} {}
};
}  // namespace roq

template <>
struct fmt::formatter<roq::Exception> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Exception const &value, Context &context) const {
    return value.format_to(context);
  }
};

template <>
struct fmt::formatter<roq::RuntimeError> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::RuntimeError const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<roq::Exception const &>(value));
  }
};

template <>
struct fmt::formatter<roq::SystemError> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::SystemError const &value, Context &context) const {
    return value.format_to(context);
  }
};

template <>
struct fmt::formatter<roq::RangeError> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::RangeError const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<roq::Exception const &>(value));
  }
};

template <>
struct fmt::formatter<roq::OverflowError> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::OverflowError const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<roq::Exception const &>(value));
  }
};

template <>
struct fmt::formatter<roq::LogicError> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::LogicError const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<roq::Exception const &>(value));
  }
};

template <>
struct fmt::formatter<roq::InvalidArgument> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::InvalidArgument const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<roq::Exception const &>(value));
  }
};

template <>
struct fmt::formatter<roq::OutOfRange> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::OutOfRange const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<roq::Exception const &>(value));
  }
};

template <>
struct fmt::formatter<roq::LengthError> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::LengthError const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<roq::Exception const &>(value));
  }
};

template <>
struct fmt::formatter<roq::Fatal> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Fatal const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<roq::Exception const &>(value));
  }
};

template <>
struct fmt::formatter<roq::FileDoesNotExist> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::FileDoesNotExist const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<roq::Exception const &>(value));
  }
};

template <>
struct fmt::formatter<roq::NotReady> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::NotReady const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<roq::Exception const &>(value));
  }
};

template <>
struct fmt::formatter<roq::BadState> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::BadState const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<roq::Exception const &>(value));
  }
};

template <>
struct fmt::formatter<roq::NetworkError> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::NetworkError const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<roq::Exception const &>(value));
  }
};

template <>
struct fmt::formatter<roq::TransportError> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::TransportError const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<roq::Exception const &>(value));
  }
};

template <>
struct fmt::formatter<roq::NotConnected> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::NotConnected const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<roq::Exception const &>(value));
  }
};

template <>
struct fmt::formatter<roq::ConnectionRefused> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::ConnectionRefused const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<roq::Exception const &>(value));
  }
};

template <>
struct fmt::formatter<roq::TimedOut> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::TimedOut const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<roq::Exception const &>(value));
  }
};

template <>
struct fmt::formatter<roq::SessionError> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::SessionError const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<roq::Exception const &>(value));
  }
};

template <>
struct fmt::formatter<roq::PermissionDenied> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::PermissionDenied const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<roq::Exception const &>(value));
  }
};

template <>
struct fmt::formatter<roq::OrderNotLive> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::OrderNotLive const &value, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(context.out(), "{}"_cf, static_cast<roq::Exception const &>(value));
  }
};
