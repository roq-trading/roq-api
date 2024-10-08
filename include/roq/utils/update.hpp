/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/utils/compare.hpp"

// update result
// - allows the user to detect changes
// - drops NaN (floating point)
// - convenience extensions to capture max-of or first-of

namespace roq {
namespace utils {

template <typename T, typename U>
constexpr bool update(T &result, U const &value) {
  using lhs_type = typename std::decay<T>::type;
  using rhs_type = typename std::decay<U>::type;
  if constexpr (std::is_floating_point<lhs_type>::value && std::is_same<lhs_type, rhs_type>::value) {
    // special case for floating point: also drop NaN
    if (compare(result, value) == std::strong_ordering::equal || compare(value, NaN) == std::strong_ordering::equal)
      return false;
  } else {
    if (compare(result, value) == std::strong_ordering::equal)
      return false;
  }
  result = value;
  return true;
}

template <typename T, typename U>
constexpr bool update_min(T &result, U const &value) {
  if (!(compare(value, result) == std::strong_ordering::less))
    return false;
  result = value;
  return true;
}

template <typename T, typename U>
constexpr bool update_max(T &result, U const &value) {
  if (!(compare(value, result) == std::strong_ordering::greater))
    return false;
  result = value;
  return true;
}

template <typename T, typename U>
constexpr bool update_first(T &result, U const &value) {
  if (compare(result, T{}) != std::strong_ordering::equal)
    return false;
  if (compare(value, U{}) != std::strong_ordering::equal)
    result = value;
  return true;
}

template <typename T, typename U>
constexpr bool update_if_not_empty(T &result, U const &value) {
  using lhs_type = typename std::decay<T>::type;
  using rhs_type = typename std::decay<U>::type;
  if constexpr (is_string<rhs_type>::value) {
    if (std::empty(value))
      return false;
  } else if constexpr (std::is_floating_point<lhs_type>::value && std::is_same<lhs_type, rhs_type>::value) {
    // update() will do the right thing
  } else {
    if (compare(value, U{}) == std::strong_ordering::equal)
      return false;
  }
  return update(result, value);
}

}  // namespace utils
}  // namespace roq
