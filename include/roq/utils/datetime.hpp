/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <cassert>
#include <chrono>

namespace roq {
namespace utils {

// wrapper class to format std::chrono durations as iso 8601

template <typename T>
struct DateTime_iso8601 final {
  explicit DateTime_iso8601(const T &value) : value_(value) {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace std::literals;
    std::chrono::sys_days days{std::chrono::duration_cast<std::chrono::days>(value_)};
    std::chrono::year_month_day ymd{days};
    auto tmp = std::chrono::time_point_cast<T>(days);
    auto tmp2 = std::chrono::duration_cast<T>(tmp.time_since_epoch());
    auto tmp3 = value_ - tmp2;
    std::chrono::hh_mm_ss hms{tmp3};
    if constexpr (
        std::is_same_v<std::decay_t<T>, std::chrono::days> || std::is_same_v<std::decay_t<T>, std::chrono::seconds>) {
      return fmt::format_to(
          context.out(),
          "{:04}-{:02}-{:02}T{:02}:{:02}:{:02}Z"sv,
          static_cast<int>(ymd.year()),
          static_cast<unsigned>(ymd.month()),
          static_cast<unsigned>(ymd.day()),
          hms.hours().count(),
          hms.minutes().count(),
          hms.seconds().count());
    } else if constexpr (std::is_same_v<std::decay_t<T>, std::chrono::milliseconds>) {
      auto remain = value_.count() % 1000u;
      return fmt::format_to(
          context.out(),
          "{:04}-{:02}-{:02}T{:02}:{:02}:{:02}.{:03}Z"sv,
          static_cast<int>(ymd.year()),
          static_cast<unsigned>(ymd.month()),
          static_cast<unsigned>(ymd.day()),
          hms.hours().count(),
          hms.minutes().count(),
          hms.seconds().count(),
          remain);
    } else if constexpr (std::is_same_v<std::decay_t<T>, std::chrono::microseconds>) {
      auto remain = value_.count() % 1000000u;
      return fmt::format_to(
          context.out(),
          "{:04}-{:02}-{:02}T{:02}:{:02}:{:02}.{:06}Z"sv,
          static_cast<int>(ymd.year()),
          static_cast<unsigned>(ymd.month()),
          static_cast<unsigned>(ymd.day()),
          hms.hours().count(),
          hms.minutes().count(),
          hms.seconds().count(),
          remain);
    } else if constexpr (std::is_same_v<std::decay_t<T>, std::chrono::nanoseconds>) {
      auto remain = value_.count() % 1000000000u;
      return fmt::format_to(
          context.out(),
          "{:04}-{:02}-{:02}T{:02}:{:02}:{:02}.{:09}Z"sv,
          static_cast<int>(ymd.year()),
          static_cast<unsigned>(ymd.month()),
          static_cast<unsigned>(ymd.day()),
          hms.hours().count(),
          hms.minutes().count(),
          hms.seconds().count(),
          remain);
    } else {
      assert(false);
      return context.out();
    }
  }

 private:
  const T value_;
};

}  // namespace utils
}  // namespace roq

template <typename T>
struct fmt::formatter<roq::utils::DateTime_iso8601<T>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::utils::DateTime_iso8601<T> const &value, Context &context) {
    return value.format_to(context);
  }
};
