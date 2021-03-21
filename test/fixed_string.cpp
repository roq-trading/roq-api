/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/fixed_string.h"

using namespace roq;
using namespace roq::literals;

TEST(fixed_string, empty) {
  roq::fixed_string<4> s;
  EXPECT_EQ(s.size(), 4u);
  EXPECT_EQ(s.length(), 0u);
  auto sv = static_cast<std::string_view>(s);
  EXPECT_TRUE(sv.empty());
  EXPECT_EQ(sv.length(), 0u);
}

TEST(fixed_string, partial) {
  constexpr auto text = "12"_sv;
  roq::fixed_string<4> s = text;
  EXPECT_EQ(s.size(), 4u);
  EXPECT_EQ(s.length(), 2u);
  auto sv = static_cast<std::string_view>(s);
  EXPECT_FALSE(sv.empty());
  EXPECT_EQ(sv.length(), 2u);
  EXPECT_EQ(sv, text);
}

TEST(fixed_string, full) {
  constexpr auto text = "123"_sv;
  roq::fixed_string<4> s = text;
  EXPECT_EQ(s.size(), 4u);
  EXPECT_EQ(s.length(), 3u);
  auto sv = static_cast<std::string_view>(s);
  EXPECT_FALSE(sv.empty());
  EXPECT_EQ(sv.length(), 3u);
  EXPECT_EQ(sv, text);
}

TEST(fixed_string, construct) {
  roq::fixed_string<4>();
  roq::fixed_string<4>("1"_sv);
  roq::fixed_string<4>("12"_sv);
  roq::fixed_string<4>("123"_sv);
  roq::fixed_string<4>("1234"_sv);
  EXPECT_THROW(roq::fixed_string<4>("12345"_sv), std::length_error);
}
