/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <cmath>
#include <cstdint>

namespace roq {
namespace utils {

// power_of_two(value)
// references:
//   https://stackoverflow.com/a/3638454
template <typename T>
static constexpr bool power_of_two(T value) {
  return value && !(value & (value - 1));
}

// round_up<multiple>(value)
// references:
//   https://stackoverflow.com/a/3407254
// - optimized version (power of two)
template <size_t multiple, typename T>
typename std::enable_if<power_of_two(multiple), T>::type static constexpr round_up(T value) {
  static_assert(multiple != T{});
  return (value + multiple - T{1}) & -multiple;
}
// - normal case
template <size_t multiple, typename T>
typename std::enable_if<!power_of_two(multiple), T>::type static constexpr round_up(T value) {
  static_assert(multiple != T{});
  T remainder = value % multiple;
  return value + (remainder ? (multiple - remainder) : T{});
}

// round_down<multiple>(value)
// - optimized case (power of two)
template <size_t multiple, typename T>
typename std::enable_if<power_of_two(multiple), T>::type static constexpr round_down(T value) {
  static_assert(multiple != T{});
  return (value) & -multiple;
}
// -- normal case
template <size_t multiple, typename T>
typename std::enable_if<!power_of_two(multiple), T>::type static constexpr round_down(T value) {
  static_assert(multiple != T{});
  T remainder = value % multiple;
  return value - remainder;
}

// round_up_next_pow_2(value)
// references:
//   https://stackoverflow.com/a/466242
template <typename T>
typename std::enable_if<std::is_unsigned<T>::value, T>::type inline constexpr round_up_next_pow_2(T value) {
  value--;
  value |= value >> 1;
  value |= value >> 2;
  value |= value >> 4;
  if constexpr (sizeof(value) >= 2)
    value |= value >> 8;
  if constexpr (sizeof(value) >= 4)
    value |= value >> 16;
  if constexpr (sizeof(value) >= 8)
    value |= value >> 32;
  value++;
  return value;
}

}  // namespace utils
}  // namespace roq
