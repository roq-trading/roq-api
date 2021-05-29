/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include <gtest/gtest.h>

#include <stdexcept>

#include "roq/utils/safe_cast.h"

using namespace roq;
using namespace roq::utils;
using namespace roq::literals;

TEST(safe_cast, int_to_int) {
  // int32_t --> int16_t
  EXPECT_EQ(static_cast<int16_t>(safe_cast<int32_t>(-32768)), -32768);
  EXPECT_EQ(static_cast<int16_t>(safe_cast<int32_t>(-1)), -1);
  EXPECT_EQ(static_cast<int16_t>(safe_cast<int32_t>(0)), 0);
  EXPECT_EQ(static_cast<int16_t>(safe_cast<int32_t>(1)), 1);
  EXPECT_EQ(static_cast<int16_t>(safe_cast<int32_t>(32767)), 32767);
  // ... overflow
  EXPECT_THROW((void)static_cast<int16_t>(safe_cast<int32_t>(-32769)), std::overflow_error);
  EXPECT_THROW((void)static_cast<int16_t>(safe_cast<int32_t>(32768)), std::overflow_error);
  // int16_t --> int32_t
  EXPECT_EQ(static_cast<int32_t>(safe_cast<int16_t>(-32768)), -32768);
  EXPECT_EQ(static_cast<int32_t>(safe_cast<int16_t>(-1)), -1);
  EXPECT_EQ(static_cast<int32_t>(safe_cast<int16_t>(0)), 0);
  EXPECT_EQ(static_cast<int32_t>(safe_cast<int16_t>(1)), 1);
  EXPECT_EQ(static_cast<int32_t>(safe_cast<int16_t>(32767)), 32767);
}

TEST(safe_cast, float_to_int) {
  // double --> int64_t
  // EXPECT_EQ(static_cast<int64_t>(safe_cast<double>(-9223372036854775807.0)),
  // -9223372036854775807);
  EXPECT_EQ(static_cast<int64_t>(safe_cast<double>(-9000000000000000000.0)), -9000000000000000000);
  EXPECT_EQ(static_cast<int64_t>(safe_cast<double>(-1.0)), -1);
  EXPECT_EQ(static_cast<int64_t>(safe_cast<double>(-0.0)), 0);
  EXPECT_EQ(static_cast<int64_t>(safe_cast<double>(0.0)), 0);
  EXPECT_EQ(static_cast<int64_t>(safe_cast<double>(1.0)), 1);
  EXPECT_EQ(static_cast<int64_t>(safe_cast<double>(9000000000000000000.0)), 9000000000000000000);
  // EXPECT_THROW(static_cast<int64_t>(safe_cast<double>(9223372036854775807.0)),
  // 9223372036854775807);
  // ... overflow
  EXPECT_THROW((void)static_cast<int64_t>(safe_cast<double>(-9223372036854775808.0)), std::overflow_error);
  EXPECT_THROW((void)static_cast<int64_t>(safe_cast<double>(9223372036854775808.0)), std::overflow_error);
  // have seen this
  auto tick_size = 1.0e-10;
  auto price = 100000000.0;
  EXPECT_EQ(static_cast<int64_t>(safe_cast(price / tick_size)), 1000000000000000000);
}
