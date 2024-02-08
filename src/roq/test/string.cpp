/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include <string_view>

#include <absl/hash/hash.h>

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

TEST_CASE("absl_hash_simple_4", "[string]") {
  std::array<String<4>, 5> raw{{
      String<4>(),
      String<4>("1"sv),
      String<4>("12"sv),
      String<4>("123"sv),
      String<4>("1234"sv),
  }};
  std::array<size_t, 5> raw_hash{{
      absl::HashOf(raw[0]),
      absl::HashOf(raw[1]),
      absl::HashOf(raw[2]),
      absl::HashOf(raw[3]),
      absl::HashOf(raw[4]),
  }};
  std::array<std::string_view, 5> heterogeneous{{
      ""sv,
      "1"sv,
      "12"sv,
      "123"sv,
      "1234"sv,
  }};
  std::array<size_t, 5> heterogeneous_hash{{
      absl::HashOf(heterogeneous[0]),
      absl::HashOf(heterogeneous[1]),
      absl::HashOf(heterogeneous[2]),
      absl::HashOf(heterogeneous[3]),
      absl::HashOf(heterogeneous[4]),
  }};
  // hash
  // ... same
  for (size_t i = 0; i < (std::size(raw_hash) - 1); ++i)
    for (size_t j = i + 1; j < std::size(raw_hash); ++j)
      CHECK(raw_hash[i] != raw_hash[j]);
  // ... heterogenous
  REQUIRE(std::size(raw_hash) == std::size(heterogeneous_hash));
  for (size_t i = 0; i < std::size(raw_hash); ++i)
    CHECK(raw_hash[i] == heterogeneous_hash[i]);
  // comparison
  // ... same
  for (size_t i = 0; i < std::size(raw_hash); ++i) {
    CHECK(raw[i] == heterogeneous[i]);
    CHECK(!(raw[i] != heterogeneous[i]));
  }
  for (size_t i = 0; i < (std::size(raw_hash) - 1); ++i)
    for (size_t j = i + 1; j < std::size(raw_hash); ++j) {
      CHECK(!(raw[i] == raw[j]));
      CHECK(raw[i] != raw[j]);
    }
  // ... heterogenous
  REQUIRE(std::size(raw) == std::size(heterogeneous));
  for (size_t i = 0; i < std::size(raw_hash); ++i) {
    CHECK(raw[i] == heterogeneous[i]);
    CHECK(!(raw[i] != heterogeneous[i]));
  }
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
