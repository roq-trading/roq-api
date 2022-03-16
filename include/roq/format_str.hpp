/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <array>
#include <string_view>

#include "roq/source_location.hpp"

// note!
// this class captures the source location + does compile-time format string checking (c++20, only)
// strongly inspired by the discussion here: https://github.com/fmtlib/fmt/issues/2022

namespace roq {

namespace detail {
// may truncate to N
template <std::size_t N>
struct static_string final {
#if __cplusplus >= 202002L
  consteval static_string(const std::string_view &sv)
      : length_(std::min(N, std::size(sv))), buffer_(create(sv, length_)) {}
#else
  constexpr static_string(const std::string_view &sv) : length_(std::min(N, sv.size())), buffer_(create(sv, length_)) {}
#endif

  static_string(const static_string &) = default;
  static_string(static_string &&) = delete;

  constexpr operator std::string_view() const {
#if __cplusplus >= 202002L
    return {std::data(buffer_), length_};
#else
    return {buffer_.data(), length_};
#endif
  }

 protected:
  static constexpr auto create(const std::string_view &sv, std::size_t length) {
    std::array<char, N> buffer;
    for (std::size_t i = 0; i < length; ++i)
      buffer[i] = sv[i];
    for (std::size_t i = length; i < N; ++i)
      buffer[i] = 0xEF;  // debug
    return buffer;
  }

 private:
  const std::size_t length_;
  const std::array<char, N> buffer_;
};
}  // namespace detail

template <typename... Args>
struct basic_format_str final {
  using file_name_type = detail::static_string<32>;
  template <typename T>
#if __cplusplus >= 202002L
  consteval basic_format_str(const T &str, const source_location &loc = source_location::current())  // NOLINT
#else
  constexpr basic_format_str(
      const std::string_view &str, const source_location &loc = source_location::current())  // NOLINT
#endif

      : str_(static_cast<std::string_view>(str)), file_name_(extract_basename(loc.file_name())), line_(loc.line()) {
    if constexpr (sizeof...(Args) > 0) {
      using checker =
          fmt::detail::format_string_checker<char, fmt::detail::error_handler, fmt::remove_cvref_t<Args>...>;
      fmt::detail::parse_format_string<true>(str_, checker(str_, {}));
    }
  }

  const fmt::string_view str_;
  const file_name_type file_name_;
  const std::uint32_t line_;

 private:
#if __cplusplus >= 202002L
  static consteval std::string_view extract_basename(const char *path){
#else
  static constexpr std::string_view extract_basename(const char *path) {
#endif
      if (path == nullptr) return {};
  std::string_view tmp{path};
#if __cplusplus >= 201703L
  if (std::empty(tmp))
#else
    if (tmp.empty())
#endif
    return {};
  auto pos = tmp.find_last_of('/');
#if __cplusplus >= 201703L
  if (pos == tmp.npos || pos == (std::size(tmp) - 1))
#else
    if (pos == tmp.npos || pos == (tmp.size() - 1))
#endif
    return tmp;
  return tmp.substr(++pos);
}
};  // namespace roq

template <typename... Args>
using format_str = basic_format_str<fmt::type_identity_t<Args>...>;

}  // namespace roq

template <size_t N>
struct fmt::formatter<roq::detail::static_string<N> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::detail::static_string<N> &value, Context &context) {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};
