/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/span.h"

using namespace roq;  // NOLINT

TEST(span, simple) {
  std::array<double, 3> raw{0.0, 2.0, 3.0};
  roq::span span(raw.data(), raw.size());
  span[0] = 1.0;
  EXPECT_DOUBLE_EQ(span[0], 1.0);
  EXPECT_DOUBLE_EQ(span[1], 2.0);
  EXPECT_DOUBLE_EQ(span[2], 3.0);
  roq::span<double> span_2;
  span_2 = span;
  EXPECT_DOUBLE_EQ(span_2[0], 1.0);
  EXPECT_DOUBLE_EQ(span_2[1], 2.0);
  EXPECT_DOUBLE_EQ(span_2[2], 3.0);
}

TEST(span, string_view) {
  std::vector<std::string_view> raw{
      "abc",
      "def",
  };
  roq::span span(raw.data(), raw.size());
  EXPECT_EQ(span[0], "abc");
  EXPECT_EQ(span[1], "def");
}
