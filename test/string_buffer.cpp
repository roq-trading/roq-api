/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/string_buffer.h"

using namespace roq;
using namespace std::literals;

TEST(string_buffer, empty) {
  roq::string_buffer<4> s;
  EXPECT_EQ(s.size(), 4);
  EXPECT_EQ(s.length(), 0);
  auto sv = static_cast<std::string_view>(s);
  EXPECT_TRUE(sv.empty());
  EXPECT_EQ(sv.length(), 0);
}

TEST(string_buffer, partial) {
  constexpr auto text = "12"sv;
  roq::string_buffer<4> s = text;
  EXPECT_EQ(s.size(), 4);
  EXPECT_EQ(s.length(), 2);
  auto sv = static_cast<std::string_view>(s);
  EXPECT_FALSE(sv.empty());
  EXPECT_EQ(sv.length(), 2);
  EXPECT_EQ(sv, text);
}

TEST(string_buffer, almost_full) {
  constexpr auto text = "123"sv;
  roq::string_buffer<4> s = text;
  EXPECT_EQ(s.size(), 4);
  EXPECT_EQ(s.length(), 3);
  auto sv = static_cast<std::string_view>(s);
  EXPECT_FALSE(sv.empty());
  EXPECT_EQ(sv.length(), 3);
  EXPECT_EQ(sv, text);
}

TEST(string_buffer, full) {
  constexpr auto text = "1234"sv;
  roq::string_buffer<4> s = text;
  EXPECT_EQ(s.size(), 4);
  EXPECT_EQ(s.length(), 4);
  auto sv = static_cast<std::string_view>(s);
  EXPECT_FALSE(sv.empty());
  EXPECT_EQ(sv.length(), 4);
  EXPECT_EQ(sv, text);
}

TEST(string_buffer, construct) {
  roq::string_buffer<4>();
  roq::string_buffer<4>("1"sv);
  roq::string_buffer<4>("12"sv);
  roq::string_buffer<4>("123"sv);
  roq::string_buffer<4>("1234"sv);
  EXPECT_THROW(roq::string_buffer<4>("12345"sv), LengthError);
}

TEST(string_buffer, push_back) {
  roq::string_buffer<4> s;
  EXPECT_EQ(s.length(), 0);
  s.push_back('1');
  EXPECT_EQ(s.length(), 1);
  s.push_back('2');
  EXPECT_EQ(s.length(), 2);
  s.push_back('3');
  EXPECT_EQ(s.length(), 3);
  s.push_back('4');
  EXPECT_EQ(s.length(), 4);
  EXPECT_THROW(s.push_back('5'), LengthError);
}
