/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include <gtest/gtest.h>

#include <span>

using namespace std::literals;

TEST(span, simple) {
  // initialize empty
  std::vector<double> empty;
  std::span empty_span(empty);
  EXPECT_TRUE(std::empty(empty_span));
  // initialize non-empty
  std::array<double, 3> raw{0.0, 2.0, 3.0};
  std::span span(raw);
  EXPECT_FALSE(std::empty(span));
  span[0] = 1.0;
  EXPECT_DOUBLE_EQ(span[0], 1.0);
  EXPECT_DOUBLE_EQ(span[1], 2.0);
  EXPECT_DOUBLE_EQ(span[2], 3.0);
  // assignment
  std::span<double> span_2;
  EXPECT_TRUE(std::empty(span_2));
  span_2 = span;
  EXPECT_FALSE(std::empty(span_2));
  EXPECT_DOUBLE_EQ(span_2[0], 1.0);
  EXPECT_DOUBLE_EQ(span_2[1], 2.0);
  EXPECT_DOUBLE_EQ(span_2[2], 3.0);
}

TEST(span, string_view) {
  std::vector<std::string_view> raw{
      "abc"sv,
      "def"sv,
  };
  std::span span(raw);
  EXPECT_EQ(span[0], "abc"sv);
  EXPECT_EQ(span[1], "def"sv);
}
