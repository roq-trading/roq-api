/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include "roq/utils/compare.hpp"

using namespace std::literals;

namespace roq {
namespace utils {

namespace {
constexpr bool safe_equal(double lhs, double rhs) {
  return std::bit_cast<uint64_t>(lhs) == std::bit_cast<uint64_t>(rhs);
}
}  // namespace

static_assert(!detail::isnan(0.0));
static_assert(detail::isnan(std::numeric_limits<double>::quiet_NaN()));

static_assert(safe_equal(detail::fabs(0.0), 0.0));
static_assert(safe_equal(detail::fabs(-0.0), 0.0));
static_assert(safe_equal(detail::fabs(1.0), 1.0));
static_assert(safe_equal(detail::fabs(-1.0), 1.0));

static_assert(compare(0.0, 0.0) == std::strong_ordering::equal);
static_assert(compare(1.0, -1.0) == std::strong_ordering::greater);
static_assert(compare(-1.0, 1.0) == std::strong_ordering::less);

static_assert(compare(NaN, NaN) == std::strong_ordering::equal);
static_assert(compare(0.0, NaN) == std::strong_ordering::greater);
static_assert(compare(NaN, 0.0) == std::strong_ordering::less);

static_assert(detail::sign_helper(-2) == -1);
static_assert(detail::sign_helper(-1) == -1);
static_assert(detail::sign_helper(0) == 0);
static_assert(detail::sign_helper(1) == 1);
static_assert(detail::sign_helper(2) == 1);

static_assert(detail::to_strong_ordering(-2) == std::strong_ordering::less);
static_assert(detail::to_strong_ordering(-1) == std::strong_ordering::less);
static_assert(detail::to_strong_ordering(0) == std::strong_ordering::equal);
static_assert(detail::to_strong_ordering(1) == std::strong_ordering::greater);
static_assert(detail::to_strong_ordering(2) == std::strong_ordering::greater);

static_assert(detail::ascii_to_lower('a') == 'a');
static_assert(detail::ascii_to_lower('z') == 'z');
static_assert(detail::ascii_to_lower('A') == 'a');
static_assert(detail::ascii_to_lower('Z') == 'z');

}  // namespace utils
}  // namespace roq

using namespace roq;
using namespace roq::utils;

TEST_CASE("compare_int", "[compare]") {
  CHECK(compare(0, 0) == std::strong_ordering::equal);
  CHECK(compare(0, 1) == std::strong_ordering::less);
  CHECK(compare(0, -1) == std::strong_ordering::greater);
}

TEST_CASE("compare_double", "[compare]") {
  CHECK(compare(0.0, 0.0) == std::strong_ordering::equal);
  CHECK(compare(0.0, -0.0) == std::strong_ordering::equal);
  CHECK(compare(-0.0, 0.0) == std::strong_ordering::equal);
  CHECK(compare(0.0, 1.0) == std::strong_ordering::less);
  CHECK(compare(0.0, -1.0) == std::strong_ordering::greater);
  CHECK(compare(NaN, NaN) == std::strong_ordering::equal);
  CHECK(compare(0.0, NaN) == std::strong_ordering::greater);
  CHECK(compare(NaN, 0.0) == std::strong_ordering::less);
}

TEST_CASE("compare_string_case_insensitive", "[compare]") {
  // same length
  CHECK(case_insensitive_compare(""sv, ""sv) == std::strong_ordering::equal);
  CHECK(case_insensitive_compare("abc123"sv, "ABC123"sv) == std::strong_ordering::equal);
  CHECK(case_insensitive_compare("abc123"sv, "DEF456"sv) == std::strong_ordering::less);
  CHECK(case_insensitive_compare("def456"sv, "ABC123"sv) == std::strong_ordering::greater);
  // different length
  CHECK(case_insensitive_compare(""sv, "ABC123"sv) == std::strong_ordering::less);
  CHECK(case_insensitive_compare("abc123"sv, ""sv) == std::strong_ordering::greater);
  CHECK(case_insensitive_compare("abc"sv, "ABC123"sv) == std::strong_ordering::less);
  CHECK(case_insensitive_compare("abc123"sv, "ABC"sv) == std::strong_ordering::greater);
}

TEST_CASE("compare_seconds", "[compare]") {
  using namespace std::chrono_literals;
  CHECK(compare(std::chrono::seconds{}, 0s) == std::strong_ordering::equal);
  CHECK(compare(0s, 1s) == std::strong_ordering::less);
  CHECK(compare(1s, 0s) == std::strong_ordering::greater);
  CHECK(compare(1s, 1s) == std::strong_ordering::equal);
}
