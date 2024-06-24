/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include <chrono>

#include "roq/api.hpp"

#include "roq/utils/update.hpp"

using namespace roq;
using namespace roq::utils;

using namespace std::literals;
using namespace std::chrono_literals;

using namespace Catch::literals;

// update()

TEST_CASE("update_double", "[update]") {
  double value = NaN;
  CHECK(update(value, 1.0) == true);
  CHECK(value == 1.0_a);
  CHECK(update(value, 2.0) == true);
  CHECK(value == 2.0_a);
  // shouldn't change
  CHECK(update(value, NaN) == false);
  CHECK(value == 2.0_a);
}

TEST_CASE("update_string", "[update]") {
  std::string value;
  CHECK(update(value, "foo"sv) == true);
  CHECK(value == "foo"sv);
  CHECK(update(value, "bar"sv) == true);
  CHECK(value == "bar"sv);
  CHECK(update(value, "bar"sv) == false);
  CHECK(value == "bar"sv);
}

TEST_CASE("update_seconds", "[update]") {
  using namespace std::chrono_literals;
  std::chrono::seconds value = {};
  CHECK(update(value, 1s) == true);
  CHECK(value == 1s);
  CHECK(update(value, 1s) == false);
  CHECK(value == 1s);
  CHECK(update(value, 2s) == true);
  CHECK(value == 2s);
}

// update_first()

TEST_CASE("update_int_first", "[update]") {
  int value = 0;
  CHECK(update_first(value, 1) == true);
  CHECK(value == 1);
  CHECK(update_first(value, 2) == false);
  CHECK(value == 1);
}

TEST_CASE("update_enum_first", "[update]") {
  enum class MyEnum { A, B, C } value = MyEnum::A;
  CHECK(value == MyEnum::A);
  CHECK(update_first(value, MyEnum::B) == true);
  CHECK(value == MyEnum::B);
  CHECK(update_first(value, MyEnum::C) == false);
  CHECK(value == MyEnum::B);
}

TEST_CASE("update_chrono_first", "[update]") {
  std::chrono::seconds value{};
  CHECK(value == std::chrono::seconds{});
  CHECK(update_first(value, 1s) == true);
  CHECK(value == 1s);
  CHECK(update_first(value, 2s) == false);
  CHECK(value == 1s);
}

// update_max()

TEST_CASE("update_int_max", "[update]") {
  int value = 0;
  CHECK(update_max(value, 1) == true);
  CHECK(value == 1);
  CHECK(update_max(value, 1) == false);
  CHECK(value == 1);
  CHECK(update_max(value, 0) == false);
  CHECK(value == 1);
  CHECK(update_max(value, 2) == true);
  CHECK(value == 2);
}

TEST_CASE("update_enum_max", "[update]") {
  enum class MyEnum { A, B, C } value = MyEnum::A;
  CHECK(value == MyEnum::A);
  CHECK(update_max(value, MyEnum::B) == true);
  CHECK(value == MyEnum::B);
  CHECK(update_max(value, MyEnum::B) == false);
  CHECK(value == MyEnum::B);
  CHECK(update_max(value, MyEnum::A) == false);
  CHECK(value == MyEnum::B);
  CHECK(update_max(value, MyEnum::C) == true);
  CHECK(value == MyEnum::C);
}

// update_if_not_empty()

TEST_CASE("update_string_if_not_empty", "[update]") {
  std::string result;
  utils::update_if_not_empty(result, "some_test"sv);
  CHECK(result == "some_test"sv);
  utils::update_if_not_empty(result, ""sv);
  CHECK(result == "some_test"sv);
  utils::update_if_not_empty(result, "foobar"sv);
  CHECK(result == "foobar"sv);
}

TEST_CASE("update_double_if_not_empty", "[update]") {
  double result = NaN;
  utils::update_if_not_empty(result, 1.0);
  CHECK(result == 1.0_a);
  utils::update_if_not_empty(result, NaN);
  CHECK(result == 1.0_a);
  utils::update_if_not_empty(result, 2.0);
  CHECK(result == 2.0_a);
}

TEST_CASE("update_enum_if_not_empty", "[update]") {
  enum class MyEnum { A, B, C } value = MyEnum::A;  // A = 0 = "empty"
  CHECK(value == MyEnum::A);
  CHECK(update_if_not_empty(value, MyEnum::B) == true);
  CHECK(value == MyEnum::B);
  CHECK(update_if_not_empty(value, MyEnum::A) == false);
  CHECK(value == MyEnum::B);
  CHECK(update_if_not_empty(value, MyEnum::C) == true);
  CHECK(value == MyEnum::C);
}
