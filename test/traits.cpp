/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include <gtest/gtest.h>

#include <string>
#include <unordered_set>
#include <vector>

#include "roq/traits.h"

using namespace roq;  // NOLINT

namespace {
template <typename T>
void test_is_pair(const T&) {
  static_assert(is_pair<T>::value == true);
}
template <typename T>
void test_is_not_pair(const T&) {
  static_assert(is_pair<T>::value == false);
}
}  // namespace

TEST(traits, is_pair) {
  test_is_pair(std::pair("abc", 123));
  test_is_not_pair(std::string("abc"));
}

namespace {
template <typename T>
void test_is_iterable(const T&) {
  static_assert(is_iterable<T>::value == true);
}
template <typename T>
void test_is_not_iterable(const T&) {
  static_assert(is_iterable<T>::value == false);
}
}  // namespace

TEST(traits, is_iterable) {
  test_is_iterable(std::vector {"abc", "def"});
  test_is_not_iterable(123);
}

namespace {
template <typename T>
void test_has_random_access_iterator(const T&) {
  static_assert(has_random_access_iterator<T>::value == true);
}
template <typename T>
void test_has_no_random_access_iterator(const T&) {
  static_assert(has_random_access_iterator<T>::value == false);
}
}  // namespace

TEST(traits, has_random_access_iterator) {
  test_has_random_access_iterator(std::vector {"abc", "def"});
  test_has_no_random_access_iterator(std::unordered_set<int> {1, 2});
}
