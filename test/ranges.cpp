/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/ranges.h"

using namespace roq;

TEST(ranges, iota_view) {
  size_t sum = 0u;
  for (auto i : roq::ranges::iota_view(5u, 10u))
    sum += i;
  EXPECT_EQ(sum, 35u);
}
