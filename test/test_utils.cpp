/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <gtest/gtest.h>
#include "roq/utils.h"

using namespace roq;  // NOLINT

struct Calc final {
  int jdn;
  int year;
  int month;
  int day;
};

TEST(date, calcs) {
  const Calc calcs[] = {
    { 2440588, 1970, 1, 1 },
    { 2458295, 2018, 6, 25 },
  };
  for (const auto& calc : calcs) {
    auto ymd = utils::JulianDayNumber::to_calendar(calc.jdn);
    EXPECT_EQ(ymd.year, calc.year);
    EXPECT_EQ(ymd.month, calc.month);
    EXPECT_EQ(ymd.day, calc.day);
    auto jdn = utils::JulianDayNumber::from_calendar(calc.year, calc.month, calc.day);
    EXPECT_EQ(jdn, calc.jdn);
  }
}

TEST(date, print) {
  char buffer[11];
  utils::JulianDayNumber::printf(buffer, sizeof(buffer), 2458295);
  buffer[10] = '\0';
  EXPECT_STREQ(buffer, "2018-06-25");
}

// FIXME(thraneh): this should be time since epoch (using chrono)
TEST(date_time, print) {
  uint64_t jdn = 2458295;
  uint64_t time = (((jdn * 24 + 21) * 60 + 17) * 60 + 20) * 1000000 + 123456;
  char buffer[27];
  utils::DateTime::printf(buffer, sizeof(buffer), time);
  buffer[26] = '\0';
  EXPECT_STREQ(buffer, "2018-06-25 21:17:20.123456");
}
