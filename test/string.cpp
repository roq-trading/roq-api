/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include <catch2/catch.hpp>

#include "roq/string.hpp"

using namespace roq;
using namespace std::literals;

TEST_CASE("string_buffer_empty", "[string_buffer]") {
  string<4> s;
  CHECK(s.size() == 4);
  CHECK(s.length() == 0);
  auto sv = static_cast<std::string_view>(s);
  CHECK(std::empty(sv) == true);
  CHECK(std::size(sv) == 0);
}

TEST_CASE("string_buffer_partial", "[string_buffer]") {
  constexpr auto text = "12"sv;
  string<4> s = text;
  CHECK(s.size() == 4);
  CHECK(s.length() == 2);
  auto sv = static_cast<std::string_view>(s);
  CHECK(std::empty(sv) == false);
  CHECK(std::size(sv) == 2);
  CHECK(sv == text);
}

TEST_CASE("string_buffer_almost_full", "[string_buffer]") {
  constexpr auto text = "123"sv;
  string<4> s = text;
  CHECK(s.size() == 4);
  CHECK(s.length() == 3);
  auto sv = static_cast<std::string_view>(s);
  CHECK(std::empty(sv) == false);
  CHECK(std::size(sv) == 3);
  CHECK(sv == text);
}

TEST_CASE("string_buffer_full", "[string_buffer]") {
  constexpr auto text = "1234"sv;
  string<4> s = text;
  CHECK(s.size() == 4);
  CHECK(s.length() == 4);
  auto sv = static_cast<std::string_view>(s);
  CHECK(std::empty(sv) == false);
  CHECK(std::size(sv) == 4);
  CHECK(sv == text);
}

TEST_CASE("string_buffer_construct", "[string_buffer]") {
  string<4>();
  string<4>("1"sv);
  string<4>("12"sv);
  string<4>("123"sv);
  string<4>("1234"sv);
  CHECK_THROWS_AS(string<4>("12345"sv), LengthError);
}

TEST_CASE("string_buffer_push_back", "[string_buffer]") {
  string<4> s;
  CHECK(s.length() == 0);
  s.push_back('1');
  CHECK(s.length() == 1);
  s.push_back('2');
  CHECK(s.length() == 2);
  s.push_back('3');
  CHECK(s.length() == 3);
  s.push_back('4');
  CHECK(s.length() == 4);
  CHECK_THROWS_AS(s.push_back('5'), LengthError);
}
