/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include <gtest/gtest.h>

#include <atomic>
#include <vector>

#include "roq/string.h"

using namespace roq;  // NOLINT

TEST(string, empty) {
  roq::string<4> s;
  EXPECT_EQ(s.size(), std::size_t{4});
  EXPECT_EQ(s.length(), std::size_t{});
  auto sv = static_cast<std::string_view>(s);
  EXPECT_TRUE(sv.empty());
  EXPECT_EQ(sv.length(), std::size_t{});
}

TEST(string, partial) {
  constexpr auto text = "12";
  roq::string<4> s = text;
  EXPECT_EQ(s.size(), std::size_t{4});
  EXPECT_EQ(s.length(), std::size_t{2});
  auto sv = static_cast<std::string_view>(s);
  EXPECT_FALSE(sv.empty());
  EXPECT_EQ(sv.length(), std::size_t{2});
  EXPECT_EQ(sv, text);
}

TEST(string, full) {
  constexpr auto text = "123";
  roq::string<4> s = text;
  EXPECT_EQ(s.size(), std::size_t{4});
  EXPECT_EQ(s.length(), std::size_t{3});
  auto sv = static_cast<std::string_view>(s);
  EXPECT_FALSE(sv.empty());
  EXPECT_EQ(sv.length(), std::size_t{3});
  EXPECT_EQ(sv, text);
}

TEST(string, construct) {
  roq::string<4>();
  roq::string<4>("1");
  roq::string<4>("12");
  roq::string<4>("123");
  roq::string<4>("1234");
  EXPECT_THROW(roq::string<4>("12345"), std::length_error);
}
