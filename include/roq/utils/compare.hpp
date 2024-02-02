/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <algorithm>
#include <array>
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
// std::fabs is *not* constexpr before c++23
// references:
//   https://stackoverflow.com/a/29457433
template <typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
constexpr T fabs(T x) {
#ifdef __APPLE__
  return __builtin_fabs(x);
#else
  return std::fabs(x);
#endif
}

// references:
//   https://stackoverflow.com/a/4609795
template <typename T, std::enable_if_t<std::is_signed_v<T>, int> = 0>
constexpr int sign_helper(T value) {
  return (T{} < value) - (value < T{});
}

constexpr std::strong_ordering to_strong_ordering(int sign) {
  std::array<std::strong_ordering, 3> const lookup{{
      std::strong_ordering::less,
      std::strong_ordering::equal,
      std::strong_ordering::greater,
  }};
  return lookup[sign_helper(sign) + 1];
}

template <typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
constexpr std::strong_ordering compare(T lhs, T rhs) {
  auto lhs_nan = std::isnan(lhs);
  auto rhs_nan = std::isnan(rhs);
  if (!lhs_nan) {
    if (!rhs_nan) {
      // references:
      //   https://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
      //   https://stackoverflow.com/a/77735
      auto diff = lhs - rhs;
      if (fabs<T>(diff) < std::numeric_limits<T>::min())  // subnormal?
        return std::strong_ordering::equal;
      auto err = std::numeric_limits<T>::epsilon() * fabs<T>(lhs + rhs);
      if (diff < -err)
        return std::strong_ordering::less;
      if (diff > err)
        return std::strong_ordering::greater;
      return std::strong_ordering::equal;
    } else {
      return std::strong_ordering::greater;
    }
  } else if (!rhs_nan) {
    return std::strong_ordering::less;
  } else {
    return std::strong_ordering::equal;
  }
}

// note! std::tolower *not* constexpr
// references:
//   https://stackoverflow.com/a/50465851
constexpr char ascii_to_lower(char value) {
  static_assert('A' < 'a');
  auto const offset = 'a' - 'A';
  if (value >= 'A' && value <= 'Z')
    return value + offset;
  return value;
}
}  // namespace detail

template <typename T, typename U>
constexpr std::strong_ordering compare(T const &lhs, U const &rhs) {
  using lhs_type = typename std::decay<T>::type;
  using rhs_type = typename std::decay<U>::type;
  if constexpr (std::is_floating_point<lhs_type>::value && std::is_same<lhs_type, rhs_type>::value) {
    return detail::compare(lhs, rhs);
  } else if constexpr (std::is_same<lhs_type, Layer>::value && std::is_same<lhs_type, rhs_type>::value) {
    if (auto result = compare(lhs.bid_price, rhs.bid_price); result != std::strong_ordering::equal)
      return result;
    if (auto result = compare(lhs.bid_quantity, rhs.bid_quantity); result != std::strong_ordering::equal)
      return result;
    if (auto result = compare(lhs.ask_price, rhs.ask_price); result != std::strong_ordering::equal)
      return result;
    if (auto result = compare(lhs.ask_quantity, rhs.ask_quantity); result != std::strong_ordering::equal)
      return result;
    return std::strong_ordering::equal;
  } else if constexpr (is_string<lhs_type>::value && is_string<rhs_type>::value) {
    // cast both to std::string_view to avoid accidential allocation of std::string
    auto sign = std::string_view{lhs}.compare(std::string_view{rhs});
    return detail::to_strong_ordering(sign);
  } else {
    if (lhs < rhs)
      return std::strong_ordering::less;
    if (lhs > rhs)
      return std::strong_ordering::greater;
    return std::strong_ordering::equal;
  }
}

// special functions

template <typename T, typename U>
typename std::enable_if<is_string<T>::value && is_string<U>::value, std::strong_ordering>::
    type constexpr case_insensitive_compare(T const &lhs, U const &rhs) {
  // cast both to std::string_view to avoid accidential allocation of std::string
  std::string_view lhs_tmp{lhs}, rhs_tmp{rhs};
  auto size1 = std::size(lhs_tmp);
  auto size2 = std::size(rhs_tmp);
  auto rlen = std::min(size1, size2);
  for (size_t i = 0; i < rlen; ++i) {
    auto l = detail::ascii_to_lower(lhs_tmp[i]);
    auto r = detail::ascii_to_lower(rhs_tmp[i]);
    if (l == r)
      continue;
    if (l < r)
      return std::strong_ordering::less;
    return std::strong_ordering::greater;
  }
  if (size1 == size2)
    return std::strong_ordering::equal;
  if (size1 < size2)
    return std::strong_ordering::less;
  return std::strong_ordering::greater;
}

template <typename T>
constexpr bool is_equal(T const &lhs, T const &rhs) {
  using value_type = typename std::decay<T>::type;
  if constexpr (std::is_floating_point<value_type>::value) {
    return detail::compare(lhs, rhs) == std::strong_ordering::equal;
  } else if constexpr (std::is_same<value_type, Layer>::value) {
    return compare(lhs, rhs) == std::strong_ordering::equal;
  } else {
    return lhs == rhs;
  }
}

template <typename T>
constexpr bool is_less(T const &lhs, T const &rhs) {
  using value_type = typename std::decay<T>::type;
  if constexpr (std::is_floating_point<value_type>::value) {
    return detail::compare(lhs, rhs) == std::strong_ordering::less;
  } else {
    return lhs < rhs;
  }
}

template <typename T>
constexpr bool is_greater(T const &lhs, T const &rhs) {
  using value_type = typename std::decay<T>::type;
  if constexpr (std::is_floating_point<value_type>::value) {
    return detail::compare(lhs, rhs) == std::strong_ordering::greater;
  } else {
    return lhs > rhs;
  }
}

template <typename T>
constexpr bool is_less_or_equal(T const &lhs, T const &rhs) {
  using value_type = typename std::decay<T>::type;
  if constexpr (std::is_floating_point<value_type>::value) {
    return !(detail::compare(lhs, rhs) == std::strong_ordering::greater);
  } else {
    return lhs <= rhs;
  }
}

template <typename T>
constexpr bool is_greater_or_equal(T const &lhs, T const &rhs) {
  using value_type = typename std::decay<T>::type;
  if constexpr (std::is_floating_point<value_type>::value) {
    return !(detail::compare(lhs, rhs) == std::strong_ordering::less);
  } else {
    return lhs >= rhs;
  }
}

template <typename T>
constexpr bool is_zero(T const &value) {
  using value_type = typename std::decay<T>::type;
  if constexpr (std::is_floating_point<value_type>::value) {
    return detail::compare(value, 0.0) == std::strong_ordering::equal;
  } else {
    return value == T{};
  }
}

template <typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = 0>
constexpr bool is_negative(T const &value) {
  return std::signbit(value);
}

}  // namespace utils
}  // namespace roq
