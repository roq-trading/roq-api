/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <cstdint>

namespace roq {

// preparing for c++20
// references:
//   https://stackoverflow.com/a/34406054

struct source_location final {
  // note! can't do consteval until c++20
  static /*consteval*/ constexpr source_location current(
      char const *file = __builtin_FILE(),
      std::int_least32_t line = __builtin_LINE(),
      char const *function = __builtin_FUNCTION()) noexcept {
    return source_location(file, line, function);
  }

  constexpr char const *file_name() const noexcept { return file_; }
  constexpr std::uint_least32_t line() const noexcept { return line_; }
  constexpr char const *function_name() const noexcept { return function_; }

 protected:
  constexpr source_location(char const *file, std::int_least32_t line, char const *function)
      : file_(file), line_(line), function_(function) {}

 private:
  char const *const file_;
  std::uint_least32_t const line_;
  char const *const function_;
};

}  // namespace roq
