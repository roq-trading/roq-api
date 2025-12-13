/* Copyright (c) 2017-2026, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include <string_view>

#include "roq/string.hpp"

using namespace std::literals;

using namespace roq;

static_assert(String<4>{"12"sv} < "2"sv);
static_assert(String<4>{"12"sv} == "12"sv);
static_assert(String<4>{"2"sv} > "12"sv);

TEST_CASE("string_empty", "[string]") {
  String<4> str;
  CHECK(str.size() == 4);
  CHECK(str.length() == 0);  // NOLINT(readability-container-size-empty)
  CHECK(std::empty(str));
  auto view = static_cast<std::string_view>(str);
  CHECK(std::empty(view) == true);
  CHECK(std::size(view) == 0);
}

TEST_CASE("string_partial", "[string]") {
  constexpr auto text = "12"sv;
  String<4> str = text;
  CHECK(str.size() == 4);
  CHECK(str.length() == 2);
  auto view = static_cast<std::string_view>(str);
  CHECK(std::empty(view) == false);
  CHECK(std::size(view) == 2);
  CHECK(view == text);
}

TEST_CASE("string_almost_full", "[string]") {
  constexpr auto text = "123"sv;
  String<4> str = text;
  CHECK(str.size() == 4);
  CHECK(str.length() == 3);
  auto view = static_cast<std::string_view>(str);
  CHECK(std::empty(view) == false);
  CHECK(std::size(view) == 3);
  CHECK(view == text);
}

TEST_CASE("string_full", "[string]") {
  constexpr auto text = "1234"sv;
  String<4> str = text;
  CHECK(str.size() == 4);
  CHECK(str.length() == 4);
  auto view = static_cast<std::string_view>(str);
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
  String<4> str;
  CHECK(str.length() == 0);  // NOLINT(readability-container-size-empty)
  CHECK(std::empty(str));
  CHECK(str == ""sv);  // NOLINT(readability-container-size-empty)
  str.push_back('1');
  CHECK(str.length() == 1);
  CHECK(str == "1"sv);
  str.push_back('2');
  CHECK(str.length() == 2);
  CHECK(str == "12"sv);
  str.push_back('3');
  CHECK(str.length() == 3);
  CHECK(str == "123"sv);
  str.push_back('4');
  CHECK(str.length() == 4);
  CHECK(str == "1234"sv);
  CHECK_THROWS_AS(str.push_back('5'), LengthError);
}

TEST_CASE("string_signed_unsigned_issue", "[string]") {
  constexpr auto text =
      "01234567890123456789012345678901234567890123456789012345678901234567890123456789"
      "01234567890123456789012345678901234567890123456789012345678901234567890123456789"sv;
  String<192> str = text;
  CHECK(str.size() == 192);
  CHECK(str.length() == 160);
  auto view = static_cast<std::string_view>(str);
  CHECK(std::empty(view) == false);
  CHECK(std::size(view) == 160);
  CHECK(view == text);
}

TEST_CASE("string_spaceship", "[string]") {
  String<8> str = "1234"sv;
  // String<8>
  CHECK(str == String<8>{"1234"sv});
  CHECK(str != String<8>{"2345"sv});
  CHECK(str < String<8>{"2345"sv});
  CHECK(str <= String<8>{"2345"sv});
  CHECK(str > String<8>{"0123"sv});
  CHECK(str >= String<8>{"0123"sv});
  CHECK(str != String<8>{});  // NOLINT(readability-container-size-empty)
  // String<16>
  CHECK(str == String<16>{"1234"sv});
  CHECK(str != String<16>{"2345"sv});
  CHECK(str < String<16>{"2345"sv});
  CHECK(str <= String<16>{"2345"sv});
  CHECK(str > String<16>{"0123"sv});
  CHECK(str >= String<16>{"0123"sv});
  CHECK(str != String<16>{});  // NOLINT(readability-container-size-empty)
  // String<4>
  CHECK(str == String<4>{"1234"sv});
  CHECK(str != String<4>{"2345"sv});
  CHECK(str < String<4>{"2345"sv});
  CHECK(str <= String<4>{"2345"sv});
  CHECK(str > String<4>{"0123"sv});
  CHECK(str >= String<4>{"0123"sv});
  CHECK(str != String<4>{});  // NOLINT(readability-container-size-empty)
  // std::string_view
  CHECK(str == "1234"sv);
  CHECK(str != "2345"sv);
  CHECK(str < "2345"sv);
  CHECK(str <= "2345"sv);
  CHECK(str > "0123"sv);
  CHECK(str >= "0123"sv);
  CHECK(str != std::string_view{});  // NOLINT(readability-container-size-empty)
}
