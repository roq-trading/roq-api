/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/string_buffer.h"

using namespace roq;
using namespace roq::literals;

TEST(string_buffer, empty) {
  roq::string_buffer<4> s;
  EXPECT_EQ(s.size(), 4);
  EXPECT_EQ(s.length(), 0);
  auto sv = static_cast<std::string_view>(s);
  EXPECT_TRUE(sv.empty());
  EXPECT_EQ(sv.length(), 0);
}

TEST(string_buffer, partial) {
  constexpr auto text = "12"_sv;
  roq::string_buffer<4> s = text;
  EXPECT_EQ(s.size(), 4);
  EXPECT_EQ(s.length(), 2);
  auto sv = static_cast<std::string_view>(s);
  EXPECT_FALSE(sv.empty());
  EXPECT_EQ(sv.length(), 2);
  EXPECT_EQ(sv, text);
}

TEST(string_buffer, full) {
  constexpr auto text = "123"_sv;
  roq::string_buffer<4> s = text;
  EXPECT_EQ(s.size(), 4);
  EXPECT_EQ(s.length(), 3);
  auto sv = static_cast<std::string_view>(s);
  EXPECT_FALSE(sv.empty());
  EXPECT_EQ(sv.length(), 3);
  EXPECT_EQ(sv, text);
}

TEST(string_buffer, construct) {
  roq::string_buffer<4>();
  roq::string_buffer<4>("1"_sv);
  roq::string_buffer<4>("12"_sv);
  roq::string_buffer<4>("123"_sv);
  roq::string_buffer<4>("1234"_sv);
  EXPECT_THROW(roq::string_buffer<4>("12345"_sv), LengthError);
}
