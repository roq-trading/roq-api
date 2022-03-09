/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>

#include "roq/compat.hpp"
#include "roq/format_str.hpp"
#include "roq/source_location.hpp"

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
struct ROQ_PUBLIC Exception : public std::exception {
  template <typename... Args>
  explicit Exception(const format_str<Args...> &fmt, Args &&...args)
      : file_name_(fmt.file_name_), line_(fmt.line_),
        what_(
            fmt.str_.size() == 0 ? std::string{}
                                 : fmt::vformat(fmt.str_, fmt::make_format_args(std::forward<Args>(args)...))) {}

  char const *what() const noexcept override { return what_.c_str(); }

  virtual const std::string_view file() const noexcept { return file_name_; }
  virtual int line() const noexcept { return line_; }

 private:
  const detail::static_string<32> file_name_;
  const int line_;
  const std::string what_;
};

//! Runtime error
struct ROQ_PUBLIC RuntimeError : public Exception {
  using Exception::Exception;
};

//! SystemError
struct ROQ_PUBLIC SystemError : public RuntimeError {
 public:
  const std::error_code &code() const noexcept { return ec_; }

  template <typename... Args>
  SystemError(std::error_code ec, const format_str<Args...> &fmt, Args &&...args)
      : RuntimeError(fmt, std::forward<Args>(args)...), ec_(ec) {}

 private:
  const std::error_code ec_;
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

}  // namespace roq

template <>
struct fmt::formatter<roq::Exception> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::Exception &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(
        ctx.out(),
        R"({{)"
        R"(type="{}", )"
        R"(what="{}", )"
        R"(file="{}", )"
        R"(line={})"
        R"(}})"sv,
        typeid(value).name(),
        value.what(),
        value.file(),
        value.line());
  }
};

template <>
struct fmt::formatter<roq::RuntimeError> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::RuntimeError &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(ctx.out(), "{}"sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::SystemError> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::SystemError &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(ctx.out(), "{}"sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::RangeError> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::RangeError &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(ctx.out(), "{}"sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::OverflowError> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::OverflowError &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(ctx.out(), "{}"sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::LogicError> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::LogicError &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(ctx.out(), "{}"sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::InvalidArgument> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::InvalidArgument &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(ctx.out(), "{}"sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::OutOfRange> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::OutOfRange &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(ctx.out(), "{}"sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::LengthError> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::LengthError &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(ctx.out(), "{}"sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::Fatal> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::Fatal &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(ctx.out(), "{}"sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::FileDoesNotExist> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::FileDoesNotExist &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(ctx.out(), "{}"sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::NotReady> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::NotReady &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(ctx.out(), "{}"sv, static_cast<const roq::Exception &>(value));
  }
};

template <>
struct fmt::formatter<roq::BadState> {
  template <typename Context>
  constexpr auto parse(Context &ctx) {
    return std::begin(ctx);
  }
  template <typename Context>
  auto format(const roq::BadState &value, Context &ctx) {
    using namespace std::literals;
    return fmt::format_to(ctx.out(), "{}"sv, static_cast<const roq::Exception &>(value));
  }
};
