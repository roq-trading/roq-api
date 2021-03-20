/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

// goals:
// - require the use of format strings (reason: making compile checks possible)
// - perhaps support both fmt::format and std::format

#include <fmt/format.h>

#include <string>
#include <string_view>
#include <utility>

#include "roq/format_str.h"
#include "roq/literals.h"

namespace roq {

namespace detail {
// note!
// FMT_STRING wraps FMT_STRING_IMPL takes a macro argument which is
// used only inside a struct
// it is not possible to access a variable from the surrounding scpe
// the argument is expected to be a string
// this work-around tries to replicate the inner workings of the macro,
// but it doesn't "work" -- the trick is something about instantiating
// a struct
inline constexpr auto compile(const format_str &fmt) {
  auto sv = static_cast<std::string_view>(fmt);
  return sv;
  /*
  if constexpr (false) {  // XXX disabled for now -- need further investigation
    using value_type = std::decay_t<decltype(sv)>::value_type;
    return fmt::detail::compile_string_to_view<value_type>(sv);
  } else {
    return sv;
  }
  */
}
}  // namespace detail

// format

template <typename T, typename... Args>
inline std::string format(const T &fmt, Args &&...args) {
  if constexpr (sizeof...(args) > 0u) {
    static_assert(std::is_same<T, format_str>::value, "use the _fmt literal");
    return fmt::format(detail::compile(fmt), std::forward<Args>(args)...);
  } else {
    static_assert(std::is_same<T, std::string_view>::value, "use the _sv literal");
    return fmt::format(fmt);
  }
}

// format_to

template <typename OutputIt, typename T, typename... Args>
inline constexpr auto format_to(OutputIt out, const T &fmt, Args &&...args) {
  if constexpr (sizeof...(args) > 0u) {
    static_assert(std::is_same<T, format_str>::value, "use the _fmt literal");
    return fmt::format_to(out, detail::compile(fmt), std::forward<Args>(args)...);
  } else {
    static_assert(std::is_same<T, std::string_view>::value, "use the _sv literal");
    return fmt::format_to(out, fmt);
  }
}

// format_to_n

template <typename OutputIt, typename T, typename... Args>
inline constexpr auto format_to_n(OutputIt out, size_t n, const T &fmt, Args &&...args) {
  if constexpr (sizeof...(args) > 0u) {
    static_assert(std::is_same<T, format_str>::value, "use the _fmt literal");
    return fmt::format_to_n(out, n, detail::compile(fmt), std::forward<Args>(args)...);
  } else {
    static_assert(std::is_same<T, std::string_view>::value, "use the _sv literal");
    return fmt::format_to_n(out, n, fmt);
  }
}

// formatted_size

template <typename T, typename... Args>
inline constexpr auto formatted_size(const T &fmt, Args &&...args) {
  if constexpr (sizeof...(args) > 0u) {
    static_assert(std::is_same<T, format_str>::value, "use the _fmt literal");
    return fmt::formatted_size(detail::compile(fmt), std::forward<Args>(args)...);
  } else {
    static_assert(std::is_same<T, std::string_view>::value, "use the _sv literal");
    return fmt::formatted_size(fmt);
  }
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
