/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

// note!
// clang14 doesn't support std::source_location
// partial support may potentially arrive with clang15
// we will fail this #include for all other compilers
// note!
// clang15 only added __builtin_source_location
#ifndef __clang__
#error "should not be included"
#endif

#include <cstdint>

namespace roq {

// preparing for c++20
// references:
//   https://stackoverflow.com/a/34406054

struct source_location final {
#ifndef __clang__
  static consteval source_location current(
#else
  // note! clang12 has bug (https://stackoverflow.com/a/68790298)
  // note! can't do consteval until clang has full support
  static constexpr source_location current(
#endif
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
