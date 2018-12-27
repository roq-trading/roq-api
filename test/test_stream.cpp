/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include <gtest/gtest.h>
#include <limits>
#include <sstream>
#include "roq/stream.h"

using namespace roq;  // NOLINT

TEST(stream, layer_1) {
  Layer layer = {
      1.234,
      1234.0,
      1.234e10,
      -1.234e10
  };
  std::stringstream ss;
  ss << layer;
  auto s = ss.str();
  EXPECT_STREQ(s.c_str(),
      "{bid_price=1.234, bid_quantity=1234, ask_price=1.234e+10, ask_quantity=-1.234e+10}");
}

TEST(stream, layer_2) {
  Layer layer = {
      0.0,
      -0.0,
      std::numeric_limits<double>::quiet_NaN(),
      std::numeric_limits<double>::infinity()
  };
  std::stringstream ss;
  ss << layer;
  auto s = ss.str();
  EXPECT_STREQ(s.c_str(),
      "{bid_price=0, bid_quantity=-0, ask_price=nan, ask_quantity=inf}");
}
