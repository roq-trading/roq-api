/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <gtest/gtest.h>
#include <limits>
#include <sstream>
#include "roq/api.h"

using namespace roq;  // NOLINT

TEST(stream, double_conversion_1) {
  common::Layer layer = {
      1.234,
      1234.0,
      1.234e10,
      -1.234e10
  };
  std::stringstream ss;
  ss << layer;
  auto s = ss.str();
  EXPECT_STREQ(s.c_str(),
      "{bid_price=1.234, bid_quantity=1234, ask_price=12340000000, ask_quantity=-12340000000}");
}

TEST(stream, double_conversion_2) {
  common::Layer layer = {
      0.0,
      -0.0,
      std::numeric_limits<double>::quiet_NaN(),
      std::numeric_limits<double>::infinity()
  };
  std::stringstream ss;
  ss << layer;
  auto s = ss.str();
  EXPECT_STREQ(s.c_str(),
      "{bid_price=0, bid_quantity=0, ask_price=, ask_quantity=}");
}
