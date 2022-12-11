/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <array>
#include <string_view>

#if defined(__APPLE__)
#include "roq/source_location.hpp"
#else
#include <source_location>
namespace roq {
using source_location = std::source_location;
}
#endif

// note!
// this class captures the source location + does compile-time format string checking (c++20, only)
// strongly inspired by the discussion here: https://github.com/fmtlib/fmt/issues/2022

namespace roq {

namespace detail {
// may truncate to N
template <std::size_t N>
struct static_string final {
  // cppcheck-suppress noExplicitConstructor
  consteval static_string(std::string_view const &sv)
      : length_{std::min(N, std::size(sv))}, buffer_{create(sv, length_)} {}

  static_string(static_string const &) = default;
  static_string(static_string &&) = delete;

  constexpr operator std::string_view() const { return {std::data(buffer_), length_}; }

 protected:
  static constexpr auto create(std::string_view const &sv, std::size_t length) {
    std::array<char, N> buffer;
    for (std::size_t i = 0; i < length; ++i)
      buffer[i] = sv[i];
    for (std::size_t i = length; i < N; ++i)
      buffer[i] = static_cast<char>(0xEF);  // debug
    return buffer;
  }

 private:
  std::size_t const length_;
  std::array<char, N> const buffer_;
};
}  // namespace detail

template <typename... Args>
struct basic_format_str final {
  using file_name_type = detail::static_string<32>;
  template <typename T>
  // cppcheck-suppress noExplicitConstructor
  consteval basic_format_str(T const &str, source_location const &loc = source_location::current())  // NOLINT
      : str_{static_cast<std::string_view>(str)}, file_name_{extract_basename(loc.file_name())}, line_{loc.line()} {
    if constexpr (sizeof...(Args) > 0) {
      using checker =
          fmt::detail::format_string_checker<char, fmt::detail::error_handler, fmt::remove_cvref_t<Args>...>;
      fmt::detail::parse_format_string<true>(str_, checker{str_, {}});
    }
  }

  fmt::string_view const str_;
  file_name_type const file_name_;
  std::uint32_t const line_;

 private:
  static consteval std::string_view extract_basename(char const *path) {
    if (path == nullptr)
      return {};
    std::string_view tmp{path};
    if (std::empty(tmp))
      return tmp;
    auto pos = tmp.find_last_of('/');
    if (pos == tmp.npos || pos == (std::size(tmp) - 1))
      return tmp;
    return tmp.substr(++pos);
  }
};  // namespace roq

template <typename... Args>
using format_str = basic_format_str<fmt::type_identity_t<Args>...>;

}  // namespace roq

template <size_t N>
struct fmt::formatter<roq::detail::static_string<N>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::detail::static_string<N> const &value, Context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, static_cast<std::string_view>(value));
  }
};
