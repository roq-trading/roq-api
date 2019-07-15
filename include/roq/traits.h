/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <string>
#include <utility>

namespace roq {

// is_pair

template <typename>
struct is_pair : std::false_type {
};
template <typename T, typename U>
struct is_pair<std::pair<T, U> > : std::true_type {
};

// is_string

template <typename>
struct is_string : std::false_type {
};
template <typename T>
struct is_string<std::basic_string<T> > : std::true_type {
};

// is_iterable (SO13830158)

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

}  // namespace roq
