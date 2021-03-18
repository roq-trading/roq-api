/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/update.h"

using namespace roq;
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
