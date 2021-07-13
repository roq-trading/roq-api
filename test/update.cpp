/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include <gtest/gtest.h>

#include <chrono>

#include "roq/utils/update.h"

using namespace roq;
using namespace roq::utils;
using namespace roq::literals;

// update()

TEST(update, double) {
  double value = NaN;
  EXPECT_TRUE(update(value, 1.0));
  EXPECT_DOUBLE_EQ(value, 1.0);
  EXPECT_TRUE(update(value, 2.0));
  EXPECT_DOUBLE_EQ(value, 2.0);
  // shouldn't change
  EXPECT_FALSE(update(value, NaN));
  EXPECT_DOUBLE_EQ(value, 2.0);
}

TEST(update, string) {
  std::string value;
  EXPECT_TRUE(update(value, "foo"_sv));
  EXPECT_EQ(value, "foo"_sv);
  EXPECT_TRUE(update(value, "bar"_sv));
  EXPECT_EQ(value, "bar"_sv);
  EXPECT_FALSE(update(value, "bar"_sv));
  EXPECT_EQ(value, "bar"_sv);
}

TEST(update, seconds) {
  using namespace std::chrono_literals;
  std::chrono::seconds value = {};
  EXPECT_TRUE(update(value, 1s));
  EXPECT_EQ(value, 1s);
  EXPECT_FALSE(update(value, 1s));
  EXPECT_EQ(value, 1s);
  EXPECT_TRUE(update(value, 2s));
  EXPECT_EQ(value, 2s);
}

// update_first()

TEST(update, int_first) {
  int value = 0;
  EXPECT_EQ(value, 0);
  EXPECT_TRUE(update_first(value, 1));
  EXPECT_EQ(value, 1);
  EXPECT_FALSE(update_first(value, 2));
  EXPECT_EQ(value, 1);
}

TEST(update, enum_first) {
  enum class MyEnum { A, B, C } value = MyEnum::A;
  EXPECT_EQ(value, MyEnum::A);
  EXPECT_TRUE(update_first(value, MyEnum::B));
  EXPECT_EQ(value, MyEnum::B);
  EXPECT_FALSE(update_first(value, MyEnum::C));
  EXPECT_EQ(value, MyEnum::B);
}

TEST(update, chrono_first) {
  std::chrono::seconds value{};
  EXPECT_EQ(value, std::chrono::seconds{});
  EXPECT_TRUE(update_first(value, std::chrono::seconds{1}));
  EXPECT_EQ(value, std::chrono::seconds{1});
  EXPECT_FALSE(update_first(value, std::chrono::seconds{2}));
  EXPECT_EQ(value, std::chrono::seconds{1});
}

// update_max()

TEST(update, int_max) {
  int value = 0;
  EXPECT_EQ(value, 0);
  EXPECT_TRUE(update_max(value, 1));
  EXPECT_EQ(value, 1);
  EXPECT_FALSE(update_max(value, 1));
  EXPECT_EQ(value, 1);
  EXPECT_FALSE(update_max(value, 0));
  EXPECT_EQ(value, 1);
  EXPECT_TRUE(update_max(value, 2));
  EXPECT_EQ(value, 2);
}

TEST(update, enum_max) {
  enum class MyEnum { A, B, C } value = MyEnum::A;
  EXPECT_EQ(value, MyEnum::A);
  EXPECT_TRUE(update_max(value, MyEnum::B));
  EXPECT_EQ(value, MyEnum::B);
  EXPECT_FALSE(update_max(value, MyEnum::B));
  EXPECT_EQ(value, MyEnum::B);
  EXPECT_FALSE(update_max(value, MyEnum::A));
  EXPECT_EQ(value, MyEnum::B);
  EXPECT_TRUE(update_max(value, MyEnum::C));
  EXPECT_EQ(value, MyEnum::C);
}

// update_if_not_empty()

TEST(update, string_if_not_empty) {
  std::string result;
  utils::update_if_not_empty(result, "some_test"_sv);
  EXPECT_EQ(result, "some_test"_sv);
  utils::update_if_not_empty(result, ""_sv);
  EXPECT_EQ(result, "some_test"_sv);
  utils::update_if_not_empty(result, "foobar"_sv);
  EXPECT_EQ(result, "foobar"_sv);
}

TEST(update, double_if_not_empty) {
  double result = NaN;
  utils::update_if_not_empty(result, 1.0);
  EXPECT_DOUBLE_EQ(result, 1.0);
  utils::update_if_not_empty(result, NaN);
  EXPECT_DOUBLE_EQ(result, 1.0);
  utils::update_if_not_empty(result, 2.0);
  EXPECT_DOUBLE_EQ(result, 2.0);
}

TEST(update, enum_if_not_empty) {
  enum class MyEnum { A, B, C } value = MyEnum::A;  // A = 0 = "empty"
  EXPECT_EQ(value, MyEnum::A);
  EXPECT_TRUE(update_if_not_empty(value, MyEnum::B));
  EXPECT_EQ(value, MyEnum::B);
  EXPECT_FALSE(update_if_not_empty(value, MyEnum::A));
  EXPECT_EQ(value, MyEnum::B);
  EXPECT_TRUE(update_if_not_empty(value, MyEnum::C));
  EXPECT_EQ(value, MyEnum::C);
}
