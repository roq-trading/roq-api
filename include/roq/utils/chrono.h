/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <date/date.h>

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <utility>

#include "roq/literals.h"

namespace roq {
namespace utils {

template <typename T, typename U>
inline std::pair<date::year_month_day, date::hh_mm_ss<T>> split(U value) {
  date::sys_days date{std::chrono::duration_cast<date::days>(value)};
  date::hh_mm_ss<T> time{std::chrono::duration_cast<T>(value % std::chrono::duration_cast<U>(date::days{1}))};
  return {date, time};
}

}  // namespace utils
}  // namespace roq

template <>
struct fmt::formatter<date::year_month_day> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const date::year_month_day &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(year="{}", )"
        R"(month={}, )"
        R"(day={} )"
        R"(}})"_sv,
        static_cast<int>(value.year()),
        static_cast<unsigned>(value.month()),
        static_cast<unsigned>(value.day()));
  }
};

template <>
struct fmt::formatter<date::hh_mm_ss<std::chrono::milliseconds>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const date::hh_mm_ss<std::chrono::milliseconds> &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(hours="{}", )"
        R"(minutes={}, )"
        R"(seconds={} )"
        R"(milliseconds={} )"
        R"(}})"_sv,
        value.hours().count(),
        value.minutes().count(),
        value.seconds().count(),
        value.subseconds().count());
  }
};
