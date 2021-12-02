/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include <gtest/gtest.h>

#include <chrono>

#include "roq/utils/chrono.h"

using namespace std::chrono_literals;

namespace roq {
namespace utils {

TEST(chrono, split) {
  auto timestamp = 1622200274123456789ns;
  auto [date, time] = split<std::chrono::milliseconds>(timestamp);
  // date
  EXPECT_EQ(static_cast<int>(date.year()), 2021);
  EXPECT_EQ(static_cast<unsigned>(date.month()), 5);
  EXPECT_EQ(static_cast<unsigned>(date.day()), 28);
  // time
  EXPECT_EQ(time.hours().count(), 11);
  EXPECT_EQ(time.minutes().count(), 11);
  EXPECT_EQ(time.seconds().count(), 14);
  EXPECT_EQ(time.subseconds().count(), 123);
}

}  // namespace utils
}  // namespace roq
