/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/utils/update.h"

using namespace roq;
using namespace roq::utils;
using namespace roq::literals;

TEST(update, floating_point) {
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

TEST(utils, update_if_not_empty_string) {
  std::string result;
  utils::update_if_not_empty(result, "some_test"_sv);
  EXPECT_EQ(result, "some_test"_sv);
  utils::update_if_not_empty(result, ""_sv);
  EXPECT_EQ(result, "some_test"_sv);
  utils::update_if_not_empty(result, "foobar"_sv);
  EXPECT_EQ(result, "foobar"_sv);
}

TEST(utils, update_if_not_empty_double) {
  double result = NaN;
  utils::update_if_not_empty(result, 1.0);
  EXPECT_DOUBLE_EQ(result, 1.0);
  utils::update_if_not_empty(result, NaN);
  EXPECT_DOUBLE_EQ(result, 1.0);
  utils::update_if_not_empty(result, 2.0);
  EXPECT_DOUBLE_EQ(result, 2.0);
}
