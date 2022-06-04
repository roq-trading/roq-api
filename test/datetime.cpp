/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include "roq/utils/datetime.hpp"

using namespace roq;

using namespace std::literals;
using namespace std::chrono_literals;  // NOLINT

TEST_CASE("datetime_test_s", "[datetime]") {
  auto ymd = std::chrono::year{2020} / std::chrono::month{6} / std::chrono::day{9};
  auto hms = 13h + 14min + 15s;

  auto sd = static_cast<std::chrono::sys_days>(ymd);
  auto tmp = std::chrono::time_point_cast<std::chrono::seconds>(sd) + hms;
  auto s = std::chrono::duration_cast<std::chrono::seconds>(tmp.time_since_epoch());

  auto str = fmt::format("{}"sv, utils::DateTime_iso8601{s});
  CHECK(str == "2020-06-09T13:14:15Z"sv);
}

TEST_CASE("datetime_test_ms", "[datetime]") {
  auto ymd = std::chrono::year{2020} / std::chrono::month{6} / std::chrono::day{9};
  auto hms = 13h + 14min + 15s;
  auto frac = 123ms;

  using value_type = std::decay_t<decltype(frac)>;
  auto sd = static_cast<std::chrono::sys_days>(ymd);
  auto tmp = std::chrono::time_point_cast<value_type>(sd) + hms + frac;
  auto s = std::chrono::duration_cast<value_type>(tmp.time_since_epoch());

  auto str = fmt::format("{}"sv, utils::DateTime_iso8601{s});
  CHECK(str == "2020-06-09T13:14:15.123Z"sv);
}

TEST_CASE("datetime_test_us", "[datetime]") {
  auto ymd = std::chrono::year{2020} / std::chrono::month{6} / std::chrono::day{9};
  auto hms = 13h + 14min + 15s;
  auto frac = 123456us;

  using value_type = std::decay_t<decltype(frac)>;
  auto sd = static_cast<std::chrono::sys_days>(ymd);
  auto tmp = std::chrono::time_point_cast<value_type>(sd) + hms + frac;
  auto s = std::chrono::duration_cast<value_type>(tmp.time_since_epoch());

  auto str = fmt::format("{}"sv, utils::DateTime_iso8601{s});
  CHECK(str == "2020-06-09T13:14:15.123456Z"sv);
}

TEST_CASE("datetime_test_ns", "[datetime]") {
  auto ymd = std::chrono::year{2020} / std::chrono::month{6} / std::chrono::day{9};
  auto hms = 13h + 14min + 15s;
  auto frac = 123456789ns;

  using value_type = std::decay_t<decltype(frac)>;
  auto sd = static_cast<std::chrono::sys_days>(ymd);
  auto tmp = std::chrono::time_point_cast<value_type>(sd) + hms + frac;
  auto s = std::chrono::duration_cast<value_type>(tmp.time_since_epoch());

  auto str = fmt::format("{}"sv, utils::DateTime_iso8601{s});
  CHECK(str == "2020-06-09T13:14:15.123456789Z"sv);
}
