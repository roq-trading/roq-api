/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include <string>
#include <unordered_set>
#include <vector>

#include "roq/utils/traits.hpp"

namespace roq {
namespace utils {

static_assert(is_string<char *>::value);
static_assert(is_string<char const *>::value);
static_assert(is_string<std::string>::value);
static_assert(is_string<std::string &>::value);
static_assert(is_string<std::string const &>::value);
static_assert(is_string<std::string_view>::value);
static_assert(is_string<std::string_view &>::value);
static_assert(is_string<std::string_view const &>::value);

}  // namespace utils
}  // namespace roq

using namespace roq;
using namespace roq::utils;

namespace {
template <typename T>
void test_is_pair(const T &) {
  static_assert(is_pair<T>::value);
}
template <typename T>
void test_is_not_pair(const T &) {
  static_assert(!is_pair<T>::value);
}
}  // namespace

TEST_CASE("traits_is_pair", "[traits]") {
  test_is_pair(std::pair("abc", 123));
  test_is_not_pair(std::string("abc"));
}

namespace {
template <typename T>
void test_is_iterable(const T &) {
  static_assert(is_iterable<T>::value);
}
template <typename T>
void test_is_not_iterable(const T &) {
  static_assert(!is_iterable<T>::value);
}
}  // namespace

TEST_CASE("traits_is_iterable", "[traits]") {
  test_is_iterable(std::vector{"abc", "def"});
  test_is_not_iterable(123);
}

namespace {
template <typename T>
void test_has_random_access_iterator(const T &) {
  static_assert(has_random_access_iterator<T>::value);
}
template <typename T>
void test_has_no_random_access_iterator(const T &) {
  static_assert(!has_random_access_iterator<T>::value);
}
}  // namespace

TEST_CASE("traits_has_random_access_iterator", "[traits]") {
  test_has_random_access_iterator(std::vector{"abc", "def"});
  test_has_no_random_access_iterator(std::unordered_set<int>{1, 2});
}
