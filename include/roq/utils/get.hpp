/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <string_view>

namespace roq {
namespace utils {

// account

namespace detail {
template <typename T, typename = void>
struct has_account : std::false_type {};
template <typename T>
struct has_account<T, decltype((void)T::account, void())> : std::true_type {};
}  // namespace detail

template <typename T>
std::string_view constexpr get_account(T const &value) {
  if constexpr (detail::has_account<T>::value)
    return value.account;
  else
    return {};
}

// exchange

namespace detail {
template <typename T, typename = void>
struct has_exchange : std::false_type {};
template <typename T>
struct has_exchange<T, decltype((void)T::exchange, void())> : std::true_type {};
}  // namespace detail

template <typename T>
std::string_view constexpr get_exchange(T const &value) {
  if constexpr (detail::has_exchange<T>::value)
    return value.exchange;
  else
    return {};
}

// symbol

namespace detail {
template <typename T, typename = void>
struct has_symbol : std::false_type {};
template <typename T>
struct has_symbol<T, decltype((void)T::symbol, void())> : std::true_type {};
}  // namespace detail

template <typename T>
std::string_view constexpr get_symbol(T const &value) {
  if constexpr (detail::has_symbol<T>::value)
    return value.symbol;
  else
    return {};
}

}  // namespace utils
}  // namespace roq
