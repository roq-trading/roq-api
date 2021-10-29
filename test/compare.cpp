/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/utils/compare.h"

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

TEST(compare, int) {
  EXPECT_EQ(compare(0, 0), 0);
  EXPECT_EQ(compare(0, 1), -1);
  EXPECT_EQ(compare(0, -1), 1);
}

TEST(compare, double) {
  EXPECT_EQ(compare(0.0, 0.0), 0);
  EXPECT_EQ(compare(0.0, -0.0), 0);
  EXPECT_EQ(compare(-0.0, 0.0), 0);
  EXPECT_EQ(compare(0.0, 1.0), -1);
  EXPECT_EQ(compare(0.0, -1.0), 1);
  EXPECT_EQ(compare(NaN, NaN), 0);
  EXPECT_EQ(compare(0.0, NaN), 1);
  EXPECT_EQ(compare(NaN, 0.0), -1);
}

TEST(compare, string_case_insensitive) {
  // same length
  EXPECT_EQ(case_insensitive_compare(""sv, ""sv), 0);
  EXPECT_EQ(case_insensitive_compare("abc123"sv, "ABC123"sv), 0);
  EXPECT_EQ(case_insensitive_compare("abc123"sv, "DEF456"sv), -1);
  EXPECT_EQ(case_insensitive_compare("def456"sv, "ABC123"sv), 1);
  // different length
  EXPECT_EQ(case_insensitive_compare(""sv, "ABC123"sv), -1);
  EXPECT_EQ(case_insensitive_compare("abc123"sv, ""sv), 1);
  EXPECT_EQ(case_insensitive_compare("abc"sv, "ABC123"sv), -1);
  EXPECT_EQ(case_insensitive_compare("abc123"sv, "ABC"sv), 1);
}

TEST(compare, seconds) {
  using namespace std::chrono_literals;
  EXPECT_EQ(compare(std::chrono::seconds{}, 0s), 0);
  EXPECT_EQ(compare(0s, 1s), -1);
  EXPECT_EQ(compare(1s, 0s), 1);
  EXPECT_EQ(compare(1s, 1s), 0);
}
