/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include <catch2/catch.hpp>

#include <chrono>

#include "roq/utils/chrono.h"

using namespace std::chrono_literals;

namespace roq {
namespace utils {

TEST_CASE("chrono_split", "[chrono]") {
  auto timestamp = 1622200274123456789ns;
  auto [date, time] = split<std::chrono::milliseconds>(timestamp);
  // date
  CHECK(static_cast<int>(date.year()) == 2021);
  CHECK(static_cast<unsigned>(date.month()) == 5);
  CHECK(static_cast<unsigned>(date.day()) == 28);
  // time
  CHECK(time.hours().count() == 11);
  CHECK(time.minutes().count() == 11);
  CHECK(time.seconds().count() == 14);
  CHECK(time.subseconds().count() == 123);
}

}  // namespace utils
}  // namespace roq
