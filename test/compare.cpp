/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include <catch2/catch.hpp>

#include "roq/utils/compare.hpp"

using namespace std::literals;

namespace roq {
namespace utils {

static_assert(!detail::isnan(0.0));
static_assert(detail::isnan(std::numeric_limits<double>::quiet_NaN()));

static_assert(detail::fabs(0.0) == 0.0);
static_assert(detail::fabs(-0.0) == 0.0);
static_assert(detail::fabs(1.0) == 1.0);
static_assert(detail::fabs(-1.0) == 1.0);

static_assert(compare(0.0, 0.0) == 0);
static_assert(compare(1.0, -1.0) == 1);
static_assert(compare(-1.0, 1.0) == -1);

static_assert(compare(NaN, NaN) == 0);
static_assert(compare(0.0, NaN) == 1);
static_assert(compare(NaN, 0.0) == -1);

}  // namespace utils
}  // namespace roq

using namespace roq;
using namespace roq::utils;

TEST_CASE("compare_int", "[compare]") {
  CHECK(compare(0, 0) == 0);
  CHECK(compare(0, 1) == -1);
  CHECK(compare(0, -1) == 1);
}

TEST_CASE("compare_double", "[compare]") {
  CHECK(compare(0.0, 0.0) == 0);
  CHECK(compare(0.0, -0.0) == 0);
  CHECK(compare(-0.0, 0.0) == 0);
  CHECK(compare(0.0, 1.0) == -1);
  CHECK(compare(0.0, -1.0) == 1);
  CHECK(compare(NaN, NaN) == 0);
  CHECK(compare(0.0, NaN) == 1);
  CHECK(compare(NaN, 0.0) == -1);
}

TEST_CASE("compare_string_case_insensitive", "[compare]") {
  // same length
  CHECK(case_insensitive_compare(""sv, ""sv) == 0);
  CHECK(case_insensitive_compare("abc123"sv, "ABC123"sv) == 0);
  CHECK(case_insensitive_compare("abc123"sv, "DEF456"sv) == -1);
  CHECK(case_insensitive_compare("def456"sv, "ABC123"sv) == 1);
  // different length
  CHECK(case_insensitive_compare(""sv, "ABC123"sv) == -1);
  CHECK(case_insensitive_compare("abc123"sv, ""sv) == 1);
  CHECK(case_insensitive_compare("abc"sv, "ABC123"sv) == -1);
  CHECK(case_insensitive_compare("abc123"sv, "ABC"sv) == 1);
}

TEST_CASE("compare_seconds", "[compare]") {
  using namespace std::chrono_literals;
  CHECK(compare(std::chrono::seconds{}, 0s) == 0);
  CHECK(compare(0s, 1s) == -1);
  CHECK(compare(1s, 0s) == 1);
  CHECK(compare(1s, 1s) == 0);
}
