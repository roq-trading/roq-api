/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <string>
#include <string_view>
#include <utility>

namespace roq {

// is_char + is_char_v

template <typename T>
using is_char = std::is_same<T, char>;

template <class T>
inline constexpr bool is_char_v = is_char<T>::value;

// is_bool + is_bool_v

template <typename T>
using is_bool = std::is_same<T, bool>;

template <class T>
inline constexpr bool is_bool_v = is_bool<T>::value;

// is_integer + is_integer_v

template <typename T, typename = void>
struct is_integer : std::false_type {};

template <typename T>
struct is_integer<
  T,
  std::enable_if_t<
    std::is_integral_v<T> && !(is_bool_v<T> | is_char_v<T>)
  >
> : std::true_type {};

template <class T>
inline constexpr bool is_integer_v = is_integer<T>::value;

// is_string + is_string_v

template <typename>
struct is_string : std::false_type {};

template <typename T>
struct is_string<std::basic_string<T> > : std::true_type {};

template <typename T>
struct is_string<std::basic_string_view<T> > : std::true_type {};

template <class T>
inline constexpr bool is_string_v = is_string<T>::value;

// is_pair + is_pair_v

template <typename>
struct is_pair : std::false_type {
};
template <typename T, typename U>
struct is_pair<std::pair<T, U> > : std::true_type {
};

template <class T>
inline constexpr bool is_pair_v = is_pair<T>::value;

// is_iterable
// source: https://stackoverflow.com/questions/13830158

namespace detail {
using std::begin;
using std::end;
template <typename T>
auto is_iterable_impl(int) -> decltype(
    begin(std::declval<T&>()) != end(std::declval<T&>()),
    void(),
    ++std::declval<decltype(begin(std::declval<T&>()))&>(),
    void(*begin(std::declval<T&>())),
    std::true_type{});
template <typename T>
std::false_type is_iterable_impl(...);
}  // namespace detail

template <typename T>
using is_iterable = decltype(detail::is_iterable_impl<T>(0));

// has_random_access_iterator (SO23848011)
// source: https://stackoverflow.com/questions/23848011

template<typename T>
using has_random_access_iterator = std::is_base_of<
  std::random_access_iterator_tag,
  typename std::iterator_traits<typename T::iterator>::iterator_category>;

}  // namespace roq
