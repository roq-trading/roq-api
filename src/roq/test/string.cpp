/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include <string_view>

#include "roq/string.hpp"

using namespace roq;
using namespace std::literals;

static_assert(String<4>{"12"sv} < "2"sv);
static_assert(String<4>{"12"sv} == "12"sv);
static_assert(String<4>{"2"sv} > "12"sv);

TEST_CASE("string_empty", "[string]") {
  String<4> s;
  CHECK(s.size() == 4);
  CHECK(s.length() == 0);
  auto sv = static_cast<std::string_view>(s);
  CHECK(std::empty(sv) == true);
  CHECK(std::size(sv) == 0);
}

TEST_CASE("string_partial", "[string]") {
  constexpr auto text = "12"sv;
  String<4> s = text;
  CHECK(s.size() == 4);
  CHECK(s.length() == 2);
  auto sv = static_cast<std::string_view>(s);
  CHECK(std::empty(sv) == false);
  CHECK(std::size(sv) == 2);
  CHECK(sv == text);
}

TEST_CASE("string_almost_full", "[string]") {
  constexpr auto text = "123"sv;
  String<4> s = text;
  CHECK(s.size() == 4);
  CHECK(s.length() == 3);
  auto sv = static_cast<std::string_view>(s);
  CHECK(std::empty(sv) == false);
  CHECK(std::size(sv) == 3);
  CHECK(sv == text);
}

TEST_CASE("string_full", "[string]") {
  constexpr auto text = "1234"sv;
  String<4> s = text;
  CHECK(s.size() == 4);
  CHECK(s.length() == 4);
  auto sv = static_cast<std::string_view>(s);
  CHECK(std::empty(sv) == false);
  CHECK(std::size(sv) == 4);
  CHECK(sv == text);
}

TEST_CASE("string_construct", "[string]") {
  String<4>{};
  String<4>{"1"sv};
  String<4>{"12"sv};
  String<4>{"123"sv};
  String<4>{"1234"sv};
  CHECK_THROWS_AS(String<4>{"12345"sv}, LengthError);
}

TEST_CASE("string_push_back", "[string]") {
  String<4> s;
  CHECK(s.length() == 0);
  CHECK(s == ""sv);
  s.push_back('1');
  CHECK(s.length() == 1);
  CHECK(s == "1"sv);
  s.push_back('2');
  CHECK(s.length() == 2);
  CHECK(s == "12"sv);
  s.push_back('3');
  CHECK(s.length() == 3);
  CHECK(s == "123"sv);
  s.push_back('4');
  CHECK(s.length() == 4);
  CHECK(s == "1234"sv);
  CHECK_THROWS_AS(s.push_back('5'), LengthError);
}

TEST_CASE("string_signed_unsigned_issue", "[string]") {
  constexpr auto text =
      "01234567890123456789012345678901234567890123456789012345678901234567890123456789"
      "01234567890123456789012345678901234567890123456789012345678901234567890123456789"sv;
  String<192> s = text;
  CHECK(s.size() == 192);
  CHECK(s.length() == 160);
  auto sv = static_cast<std::string_view>(s);
  CHECK(std::empty(sv) == false);
  CHECK(std::size(sv) == 160);
  CHECK(sv == text);
}

TEST_CASE("string_spaceship", "[string]") {
  String<8> s = "1234"sv;
  CHECK(s == "1234"sv);
  CHECK(s != "2345"sv);
  CHECK(s < "2345"sv);
  CHECK(s <= "2345"sv);
  CHECK(s > "0123"sv);
  CHECK(s >= "0123"sv);
  CHECK(s != std::string_view{});
}
