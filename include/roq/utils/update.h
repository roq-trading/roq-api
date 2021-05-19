/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include "roq/utils/compare.h"

// update result
// - allows the user to detect changes
// - drops NaN (floating point)
// - convenience extensions to capture max-of or first-of

namespace roq {
namespace utils {

template <typename T, typename U>
constexpr bool update(T &result, const U &value) {
  using lhs_type = typename std::decay<T>::type;
  using rhs_type = typename std::decay<U>::type;
  if constexpr (
      std::is_floating_point<lhs_type>::value && std::is_same<lhs_type, rhs_type>::value) {
    // special case for floating point: also drop NaN
    if (compare(result, value) == 0 || compare(value, NaN) == 0)
      return false;
  } else {
    if (compare(result, value) == 0)
      return false;
  }
  result = value;
  return true;
}

template <typename T, typename U>
constexpr bool update_max(T &result, const U &value) {
  if (compare(value, result) <= 0)
    return false;
  result = value;
  return true;
}

template <typename T, typename U>
constexpr bool update_first(T &result, const U &value) {
  if (compare(result, T{}) != 0)
    return false;
  if (compare(value, U{}) != 0)
    result = value;
  return false;
}

template <typename T, typename U>
constexpr bool update_if_not_empty(T &result, const U &value) {
  using lhs_type = typename std::decay<T>::type;
  using rhs_type = typename std::decay<U>::type;
  if constexpr (is_string<lhs_type>::value && is_string<rhs_type>::value) {
    if (value.empty())
      return false;
  }
  return update(result, value);
}

}  // namespace utils
}  // namespace roq
