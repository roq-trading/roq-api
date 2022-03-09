/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <algorithm>
#include <limits>
#include <type_traits>

#include "roq/layer.hpp"

#include "roq/utils/traits.hpp"

// c++20 "spaceship"-like comparison
// - rules to deal with NaN (floating point)
// - compile-time evaluation

namespace roq {
namespace utils {

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
constexpr int compare(T lhs, T rhs) {
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
  if constexpr (std::is_floating_point<lhs_type>::value && std::is_same<lhs_type, rhs_type>::value) {
    return detail::compare(lhs, rhs);
  } else if constexpr (std::is_same<lhs_type, Layer>::value && std::is_same<lhs_type, rhs_type>::value) {
    if (auto result = compare(lhs.bid_price, rhs.bid_price) != 0)
      return result;
    if (auto result = compare(lhs.bid_quantity, rhs.bid_quantity) != 0)
      return result;
    if (auto result = compare(lhs.ask_price, rhs.ask_price) != 0)
      return result;
    if (auto result = compare(lhs.ask_quantity, rhs.ask_quantity) != 0)
      return result;
    return 0;
  } else if constexpr (is_string<lhs_type>::value && is_string<rhs_type>::value) {
    // cast both to std::string_view to avoid accidential allocation of std::string
    return std::string_view(lhs).compare(std::string_view(rhs));
  } else {
    if (lhs < rhs)
      return -1;
    if (lhs > rhs)
      return 1;
    return 0;
  }
}

// special functions

template <typename T, typename U>
typename std::enable_if<is_string<T>::value && is_string<U>::value, int>::type case_insensitive_compare(
    const T &lhs, const U &rhs) {
  // cast both to std::string_view to avoid accidential allocation of std::string
  std::string_view lhs_tmp(lhs), rhs_tmp(rhs);
  // comparing using std::toupper
  // otherwise following conventions from std::string/std::string_view
  // references:
  //   https://en.cppreference.com/w/cpp/string/basic_string/compare
  auto size1 = std::size(lhs_tmp), size2 = std::size(rhs_tmp);
  auto rlen = std::min(size1, size2);
  for (size_t i = 0; i < rlen; ++i) {
    auto l = std::toupper(lhs_tmp[i]), r = std::toupper(rhs_tmp[i]);
    if (l != r)
      return (l < r) ? -1 : 1;
  }
  if (size1 != size2)
    return size1 < size2 ? -1 : 1;
  return 0;
}

}  // namespace utils
}  // namespace roq
