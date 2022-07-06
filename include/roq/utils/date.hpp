/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <cassert>
#include <chrono>

namespace roq {
namespace utils {

// wrapper class to format std::chrono durations as iso 8601

template <typename T>
struct Date_iso8601 final {
  explicit Date_iso8601(const T &value) : value_(value) {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace std::literals;
    std::chrono::sys_days days{std::chrono::duration_cast<std::chrono::days>(value_)};
    std::chrono::year_month_day ymd{days};
    return fmt::format_to(
        context.out(),
        "{:04}-{:02}-{:02}"sv,
        static_cast<int>(ymd.year()),
        static_cast<unsigned>(ymd.month()),
        static_cast<unsigned>(ymd.day()));
  }

 private:
  const T value_;
};

}  // namespace utils
}  // namespace roq

template <typename T>
struct fmt::formatter<roq::utils::Date_iso8601<T>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::utils::Date_iso8601<T> const &value, Context &context) const {
    return value.format_to(context);
  }
};
