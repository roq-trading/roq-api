/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

// objectives:
// - require the use of format strings (reason: making compile checks possible)
// - possibly support both fmt::format and std::format

#include <fmt/format.h>

#include <string>
#include <string_view>
#include <utility>

#include "roq/literals.h"

namespace roq {

// format

template <typename... Args>
inline std::string format(const std::string_view &&fmt, Args &&...args) {
  return fmt::format(fmt, std::forward<Args>(args)...);
}

// format_to

template <typename OutputIt, typename... Args>
inline constexpr auto format_to(OutputIt out, const std::string_view &fmt, Args &&...args) {
  return fmt::format_to(out, fmt, std::forward<Args>(args)...);
}

// format_to_n

template <typename OutputIt, typename... Args>
inline constexpr auto format_to_n(OutputIt out, size_t n, const std::string_view &fmt, Args &&...args) {
  return fmt::format_to_n(out, n, fmt, std::forward<Args>(args)...);
}

// formatted_size

template <typename... Args>
inline constexpr auto formatted_size(const std::string_view &fmt, Args &&...args) {
  return fmt::formatted_size(fmt, std::forward<Args>(args)...);
}

// join

template <typename Range>
inline constexpr auto join(Range &&range, const std::string_view &sep) {
#if (FMT_VERSION < 70000)
  return fmt::join(range.begin(), range.end(), sep);
#else
  return fmt::join(range, sep);
#endif
}

// formatter

struct formatter {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
};

}  // namespace roq
