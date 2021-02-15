/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

// goals:
// - only allow type-safe format strings
// - perhaps allow for slightly easier transition to std::format

#include <fmt/format.h>

#include <string>
#include <string_view>
#include <utility>

namespace roq {

template <typename... Args>
inline std::string format(const std::string_view &format_str, Args &&... args) {
  return fmt::format(format_str, std::forward<Args>(args)...);
}

template <typename OutputIt, typename... Args>
inline constexpr OutputIt format_to(
    OutputIt out, const std::string_view &format_str, Args &&... args) {
  return fmt::format_to(out, format_str, std::forward<Args>(args)...);
}

template <typename... Args>
inline constexpr size_t formatted_size(const std::string_view &format_str, Args &&... args) {
  return fmt::formatted_size(format_str, std::forward<Args>(args)...);
}

struct formatter {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
};

}  // namespace roq
