/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include <stdexcept>

#include "roq/utils/safe_cast.hpp"

using namespace roq;
using namespace roq::utils;
using namespace std::literals;

TEST_CASE("safe_cast_int_to_int", "[safe_cast]") {
  // int32_t --> int16_t
  CHECK(static_cast<int16_t>(safe_cast<int32_t>(-32768)) == -32768);
  CHECK(static_cast<int16_t>(safe_cast<int32_t>(-1)) == -1);
  CHECK(static_cast<int16_t>(safe_cast<int32_t>(0)) == 0);
  CHECK(static_cast<int16_t>(safe_cast<int32_t>(1)) == 1);
  CHECK(static_cast<int16_t>(safe_cast<int32_t>(32767)) == 32767);
  // ... overflow
  CHECK_THROWS_AS((void)static_cast<int16_t>(safe_cast<int32_t>(-32769)), OverflowError);
  CHECK_THROWS_AS((void)static_cast<int16_t>(safe_cast<int32_t>(32768)), OverflowError);
  // int16_t --> int32_t
  CHECK(static_cast<int32_t>(safe_cast<int16_t>(-32768)) == -32768);
  CHECK(static_cast<int32_t>(safe_cast<int16_t>(-1)) == -1);
  CHECK(static_cast<int32_t>(safe_cast<int16_t>(0)) == 0);
  CHECK(static_cast<int32_t>(safe_cast<int16_t>(1)) == 1);
  CHECK(static_cast<int32_t>(safe_cast<int16_t>(32767)) == 32767);
}

TEST_CASE("safe_cast_float_to_int", "[safe_cast]") {
  // double --> int64_t
  // EXPECT_EQ(static_cast<int64_t>(safe_cast<double>(-9223372036854775807.0)),
  // -9223372036854775807);
  CHECK(static_cast<int64_t>(safe_cast<double>(-9000000000000000000.0)) == -9000000000000000000);
  CHECK(static_cast<int64_t>(safe_cast<double>(-1.0)) == -1);
  CHECK(static_cast<int64_t>(safe_cast<double>(-0.0)) == 0);
  CHECK(static_cast<int64_t>(safe_cast<double>(0.0)) == 0);
  CHECK(static_cast<int64_t>(safe_cast<double>(1.0)) == 1);
  CHECK(static_cast<int64_t>(safe_cast<double>(9000000000000000000.0)) == 9000000000000000000);
  // CHECK_THROWS_AS(static_cast<int64_t>(safe_cast<double>(9223372036854775807.0)),
  // 9223372036854775807);
  // ... overflow
  CHECK_THROWS_AS((void)static_cast<int64_t>(safe_cast<double>(-9223372036854775808.0)), OverflowError);
  CHECK_THROWS_AS((void)static_cast<int64_t>(safe_cast<double>(9223372036854775808.0)), OverflowError);
  // have seen this
  auto tick_size = 1.0e-10;
  auto price = 100000000.0;
  CHECK(static_cast<int64_t>(safe_cast(price / tick_size)) == 1000000000000000000);
}
