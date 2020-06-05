/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/string.h"

using namespace roq;  // NOLINT

TEST(string, empty) {
  roq::string<4> s;
  EXPECT_EQ(s.size(), std::size_t{3});
  EXPECT_EQ(s.length(), std::size_t{});
  auto sv = static_cast<std::string_view>(s);
  EXPECT_TRUE(sv.empty());
  EXPECT_EQ(sv.length(), std::size_t{});
}

TEST(string, partial) {
  constexpr auto text = "12";
  roq::string<4> s = text;
  EXPECT_EQ(s.size(), std::size_t{3});
  EXPECT_EQ(s.length(), std::size_t{2});
  auto sv = static_cast<std::string_view>(s);
  EXPECT_FALSE(sv.empty());
  EXPECT_EQ(sv.length(), std::size_t{2});
  EXPECT_EQ(sv, text);
}

TEST(string, full) {
  constexpr auto text = "123";
  roq::string<4> s = text;
  EXPECT_EQ(s.size(), std::size_t{3});
  EXPECT_EQ(s.length(), std::size_t{3});
  auto sv = static_cast<std::string_view>(s);
  EXPECT_FALSE(sv.empty());
  EXPECT_EQ(sv.length(), std::size_t{3});
  EXPECT_EQ(sv, text);
}

TEST(string, push_back) {
  roq::string<4> s;
  EXPECT_EQ(s.length(), std::size_t{});
  auto sv0 = static_cast<std::string_view>(s);
  EXPECT_TRUE(sv0.empty());
  s.push_back('1');
  EXPECT_EQ(s.length(), std::size_t{1});
  auto sv1 = static_cast<std::string_view>(s);
  EXPECT_EQ(sv1, "1");
  s.push_back('2');
  EXPECT_EQ(s.length(), std::size_t{2});
  auto sv2 = static_cast<std::string_view>(s);
  EXPECT_EQ(sv2, "12");
  s.push_back('3');
  EXPECT_EQ(s.length(), std::size_t{3});
  auto sv3 = static_cast<std::string_view>(s);
  EXPECT_EQ(sv3, "123");
  // should now fail with length error
  EXPECT_THROW(s.push_back('4'), std::length_error);
  EXPECT_EQ(s.length(), std::size_t{3});
  s.clear();
  EXPECT_EQ(s.length(), 0);
  auto sv = static_cast<std::string_view>(s);
  EXPECT_TRUE(sv.empty());
}

TEST(string, construct) {
  roq::string<4>();
  roq::string<4>("1");
  roq::string<4>("12");
  roq::string<4>("123");
  EXPECT_THROW(roq::string<4>("1234"), std::length_error);
}
