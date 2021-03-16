/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <limits>
#include <type_traits>

#include "roq/layer.h"

// "spaceship" comparison
// - rules to deal with NaN (floating point)
// - compile-time evaluation

namespace roq {

namespace detail {
template <typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
constexpr bool isnan(T x) {
  // std::isnan is *not* constexpr
  // references:
  //   https://en.cppreference.com/w/cpp/numeric/math/isnan
  return x != x;
}

template <typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
constexpr bool fabs(T x) {
  // references:
  //   https://stackoverflow.com/a/29457433
#if __APPLE__
  return __builtin_fabs(x);
#else
  return std::fabs(x);
#endif
}

template <typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
constexpr int spaceship(T lhs, T rhs) {
  auto lhs_nan = isnan<T>(lhs);
  auto rhs_nan = isnan<T>(rhs);
  if (!lhs_nan) {
    if (!rhs_nan) {
      // references:
      //   https://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
      //   https://stackoverflow.com/a/77735
      auto diff = lhs - rhs;
      if (fabs<T>(diff) < std::numeric_limits<T>::min())  // subnormal?
        return 0;
      auto err = std::numeric_limits<T>::epsilon() * fabs<T>(lhs + rhs);
      if (diff < -err)
        return -1;
      if (diff > err)
        return 1;
      return 0;
    } else {
      return 1;
    }
  } else if (!rhs_nan) {
    return -1;
  } else {
    return 0;
  }
}
}  // namespace detail

template <typename T, typename U>
constexpr int compare(const T &lhs, const U &rhs) {
  using lhs_type = typename std::decay<T>::type;
  using rhs_type = typename std::decay<U>::type;
  if constexpr (
      std::is_floating_point<lhs_type>::value && std::is_same<lhs_type, rhs_type>::value) {
    return detail::spaceship(lhs, rhs);
  } else if constexpr (
      std::is_same<lhs_type, Layer>::value && std::is_same<lhs_type, rhs_type>::value) {
    if (auto result = compare(lhs.bid_price, rhs.bid_price) != 0)
      return result;
    if (auto result = compare(lhs.bid_quantity, rhs.bid_quantity) != 0)
      return result;
    if (auto result = compare(lhs.ask_price, rhs.ask_price) != 0)
      return result;
    if (auto result = compare(lhs.ask_quantity, rhs.ask_quantity) != 0)
      return result;
    return 0;
  } else if constexpr (
      (std::is_same<lhs_type, std::string>::value ||
       std::is_same<lhs_type, std::string_view>::value) &&
      (std::is_same<rhs_type, std::string>::value ||
       std::is_same<rhs_type, std::string_view>::value)) {
    return lhs.compare(rhs);
  } else {
    if (lhs < rhs)
      return -1;
    if (rhs < lhs)
      return 1;
    return 0;
  }
}

}  // namespace roq
