/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/ranges.h"

using namespace roq;

TEST(ranges, iota_view) {
  size_t sum = 0;
  for (auto i : roq::ranges::iota_view(5, 10))
    sum += i;
  EXPECT_EQ(sum, 35);
}
