/* Copyright (c) 2017-2025, Hans Erik Thrane */

#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <array>
#include <iterator>
#include <span>
#include <string>
#include <string_view>
#include <vector>

using namespace std::literals;

using namespace Catch::literals;

// NOLINTBEGIN(readability-magic-numbers)

TEST_CASE("span_simple", "[span]") {
  // initialize empty
  std::vector<double> empty;
  std::span empty_span{empty};
  CHECK(std::empty(empty_span) == true);
  // initialize non-empty
  std::array<double, 3> raw{{
      0.0,
      2.0,
      3.0,
  }};
  std::span span{raw};
  CHECK(std::empty(span) == false);
  span[0] = 1.0;
  CHECK(span[0] == 1.0_a);
  CHECK(span[1] == 2.0_a);
  CHECK(span[2] == 3.0_a);
  // assignment
  std::span<double> span_2;
  CHECK(std::empty(span_2) == true);
  span_2 = span;
  CHECK(std::empty(span_2) == false);
  CHECK(span_2[0] == 1.0_a);
  CHECK(span_2[1] == 2.0_a);
  CHECK(span_2[2] == 3.0_a);
}

TEST_CASE("span_string_view", "[span]") {
  std::vector<std::string_view> raw{
      "abc"sv,
      "def"sv,
  };
  std::span span{raw};
  CHECK(span[0] == "abc"sv);
  CHECK(span[1] == "def"sv);
}

// NOLINTEND(readability-magic-numbers)
