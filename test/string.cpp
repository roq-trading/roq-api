/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include <string_view>

#include "roq/string.hpp"

using namespace std::literals;

using namespace roq;

// NOLINTBEGIN(readability-magic-numbers)

static_assert(String<4>{"12"sv} < "2"sv);
static_assert(String<4>{"12"sv} == "12"sv);
static_assert(String<4>{"2"sv} > "12"sv);

TEST_CASE("string_empty", "[string]") {
  String<4> string;
  CHECK(string.size() == 4);
  CHECK(string.length() == 0);
  auto view = static_cast<std::string_view>(string);
  CHECK(std::empty(view) == true);
  CHECK(std::size(view) == 0);
}

TEST_CASE("string_partial", "[string]") {
  constexpr auto text = "12"sv;
  String<4> string = text;
  CHECK(string.size() == 4);
  CHECK(string.length() == 2);
  auto view = static_cast<std::string_view>(string);
  CHECK(std::empty(view) == false);
  CHECK(std::size(view) == 2);
  CHECK(view == text);
}

TEST_CASE("string_almost_full", "[string]") {
  constexpr auto text = "123"sv;
  String<4> string = text;
  CHECK(string.size() == 4);
  CHECK(string.length() == 3);
  auto view = static_cast<std::string_view>(string);
  CHECK(std::empty(view) == false);
  CHECK(std::size(view) == 3);
  CHECK(view == text);
}

TEST_CASE("string_full", "[string]") {
  constexpr auto text = "1234"sv;
  String<4> string = text;
  CHECK(string.size() == 4);
  CHECK(string.length() == 4);
  auto view = static_cast<std::string_view>(string);
  CHECK(std::empty(view) == false);
  CHECK(std::size(view) == 4);
  CHECK(view == text);
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
  String<4> string;
  CHECK(string.length() == 0);
  CHECK(string == ""sv);
  string.push_back('1');
  CHECK(string.length() == 1);
  CHECK(string == "1"sv);
  string.push_back('2');
  CHECK(string.length() == 2);
  CHECK(string == "12"sv);
  string.push_back('3');
  CHECK(string.length() == 3);
  CHECK(string == "123"sv);
  string.push_back('4');
  CHECK(string.length() == 4);
  CHECK(string == "1234"sv);
  CHECK_THROWS_AS(string.push_back('5'), LengthError);
}

TEST_CASE("string_signed_unsigned_issue", "[string]") {
  constexpr auto text =
      "01234567890123456789012345678901234567890123456789012345678901234567890123456789"
      "01234567890123456789012345678901234567890123456789012345678901234567890123456789"sv;
  String<192> string = text;
  CHECK(string.size() == 192);
  CHECK(string.length() == 160);
  auto view = static_cast<std::string_view>(string);
  CHECK(std::empty(view) == false);
  CHECK(std::size(view) == 160);
  CHECK(view == text);
}

TEST_CASE("string_spaceship", "[string]") {
  String<8> string = "1234"sv;
  // String<8>
  CHECK(string == String<8>{"1234"sv});
  CHECK(string != String<8>{"2345"sv});
  CHECK(string < String<8>{"2345"sv});
  CHECK(string <= String<8>{"2345"sv});
  CHECK(string > String<8>{"0123"sv});
  CHECK(string >= String<8>{"0123"sv});
  CHECK(string != String<8>{});
  // String<16>
  CHECK(string == String<16>{"1234"sv});
  CHECK(string != String<16>{"2345"sv});
  CHECK(string < String<16>{"2345"sv});
  CHECK(string <= String<16>{"2345"sv});
  CHECK(string > String<16>{"0123"sv});
  CHECK(string >= String<16>{"0123"sv});
  CHECK(string != String<16>{});
  // String<4>
  CHECK(string == String<4>{"1234"sv});
  CHECK(string != String<4>{"2345"sv});
  CHECK(string < String<4>{"2345"sv});
  CHECK(string <= String<4>{"2345"sv});
  CHECK(string > String<4>{"0123"sv});
  CHECK(string >= String<4>{"0123"sv});
  CHECK(string != String<4>{});
  // std::string_view
  CHECK(string == "1234"sv);
  CHECK(string != "2345"sv);
  CHECK(string < "2345"sv);
  CHECK(string <= "2345"sv);
  CHECK(string > "0123"sv);
  CHECK(string >= "0123"sv);
  CHECK(string != std::string_view{});
}

// NOLINTEND(readability-magic-numbers)
