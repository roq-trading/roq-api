/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <string>
#include <string_view>
#include <utility>

namespace roq {
namespace utils {

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
    typename std::enable_if<std::is_integral<T>::value && !(is_bool<T>::value | is_char<T>::value)>::type>
    : std::true_type {};

template <class T>
inline constexpr bool is_integer_v = is_integer<T>::value;

// is_string + is_string_v
// references:
//   https://stackoverflow.com/a/57812868

#if __cplusplus >= 201703L
template <typename T>
struct is_string : public std::disjunction<
                       std::is_same<char *, typename std::decay<T>::type>,
                       std::is_same<char const *, typename std::decay<T>::type>,
                       std::is_same<std::string, typename std::decay<T>::type>,
                       std::is_same<std::string_view, typename std::decay<T>::type>> {};

template <class T>
inline constexpr bool is_string_v = is_string<T>::value;

#else
template <typename T, typename = void>
struct is_string : std::false_type {};

template <typename T>
struct is_string<
    T,
    typename std::enable_if<
        std::is_same<char *, typename std::decay<T>::type>::value ||
        std::is_same<char const *, typename std::decay<T>::type>::value ||
        std::is_same<std::string, typename std::decay<T>::type>::value ||
        std::is_same<std::string_view, typename std::decay<T>::type>::value>::type> : public std::true_type {};
#endif

// is_pair + is_pair_v

template <typename>
struct is_pair : std::false_type {};
template <typename T, typename U>
struct is_pair<std::pair<T, U>> : std::true_type {};

template <class T>
inline constexpr bool is_pair_v = is_pair<T>::value;

// is_iterable
// references:
//   https://stackoverflow.com/a/29634934

namespace detail {
using std::begin;
using std::end;
template <typename T>
auto is_iterable_impl(int) -> decltype(
    begin(std::declval<T &>()) != end(std::declval<T &>()),
    void(),
    ++std::declval<decltype(begin(std::declval<T &>())) &>(),
    void(*begin(std::declval<T &>())),
    std::true_type{});
template <typename T>
std::false_type is_iterable_impl(...);
}  // namespace detail

template <typename T>
using is_iterable = decltype(detail::is_iterable_impl<T>(0));

// has_random_access_iterator
// references:
//   https://stackoverflow.com/a/24011465

template <typename T>
using has_random_access_iterator = std::
    is_base_of<std::random_access_iterator_tag, typename std::iterator_traits<typename T::iterator>::iterator_category>;

// is_duration
// refrences:
//   https://stackoverflow.com/a/41851068

template <typename T>
struct is_duration : std::false_type {};

template <typename Rep, typename Period>
struct is_duration<std::chrono::duration<Rep, Period>> : std::true_type {};

template <class T>
inline constexpr bool is_duration_v = is_duration<T>::value;

}  // namespace utils
}  // namespace roq
