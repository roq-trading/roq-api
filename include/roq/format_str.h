/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <string_view>
#include <tuple>

#include "roq/source_location.h"

// note!
// this class captures the source location + does compile-time format string checking (c++20, only)
// strongly inspired by the discussion here: https://github.com/fmtlib/fmt/issues/2022

namespace roq {
#if __cplusplus >= 202002L
template <typename... Args>
struct basic_format_str final {
  template <typename T>
  consteval basic_format_str(const T &str, const source_location &loc = source_location::current())  // NOLINT
      : str_(static_cast<std::string_view>(str)), file_name_(extract_basename(loc.file_name())), line_(loc.line()) {
    if constexpr (sizeof...(Args) > 0) {
      using checker =
          fmt::detail::format_string_checker<char, fmt::detail::error_handler, fmt::remove_cvref_t<Args>...>;
      fmt::detail::parse_format_string<true>(str_, checker(str_, {}));
    }
  }

  const fmt::string_view str_;
  const std::string_view file_name_;
  const std::uint32_t line_;

 private:
  static consteval std::string_view extract_basename(const char *path) {
    if (path == nullptr)
      return {};
    std::string_view tmp{path};
    if (std::empty(tmp))
      return {};
    auto pos = tmp.find_last_of('/');
    if (pos == tmp.npos || pos == (std::size(tmp) - 1))
      return tmp;
    return tmp.substr(++pos);
  }
};
#else
template <typename... Args>
struct basic_format_str {
  basic_format_str(const std::string_view &str, const source_location &loc = source_location::current())  // NOLINT
      : str_(str), file_name_(extract_basename(loc.file_name())), line_(loc.line()) {}

  const fmt::string_view str_;
  const std::string_view file_name_;
  const std::uint32_t line_;

 private:
  static constexpr std::string_view extract_basename(const char *path) {
    if (path == nullptr)
      return {};
    std::string_view tmp{path};
    if (tmp.size() == 0)
      return {};
    auto pos = tmp.find_last_of('/');
    if (pos == tmp.npos || pos == (tmp.size() - 1))
      return tmp;
    return tmp.substr(++pos);
  }
};
#endif

template <typename... Args>
using format_str = basic_format_str<fmt::type_identity_t<Args>...>;

}  // namespace roq
