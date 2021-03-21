/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/literals.h"
#include "roq/span.h"

using namespace roq;
using namespace roq::literals;

TEST(span, simple) {
  // initialize empty
  std::vector<double> empty;
  roq::span empty_span(empty);
  EXPECT_TRUE(empty_span.empty());
  // initialize non-empty
  std::array<double, 3u> raw{0.0, 2.0, 3.0};
  roq::span span(raw);
  EXPECT_FALSE(span.empty());
  span[0u] = 1.0;
  EXPECT_DOUBLE_EQ(span[0u], 1.0);
  EXPECT_DOUBLE_EQ(span[1u], 2.0);
  EXPECT_DOUBLE_EQ(span[2u], 3.0);
  // assignment
  roq::span<double> span_2;
  EXPECT_TRUE(span_2.empty());
  span_2 = span;
  EXPECT_FALSE(span_2.empty());
  EXPECT_DOUBLE_EQ(span_2[0u], 1.0);
  EXPECT_DOUBLE_EQ(span_2[1u], 2.0);
  EXPECT_DOUBLE_EQ(span_2[2u], 3.0);
}

TEST(span, string_view) {
  std::vector<std::string_view> raw{
      "abc"_sv,
      "def"_sv,
  };
  roq::span span(raw);
  EXPECT_EQ(span[0u], "abc"_sv);
  EXPECT_EQ(span[1u], "def"_sv);
}
