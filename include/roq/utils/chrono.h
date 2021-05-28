/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <date/date.h>

#include <tuple>

namespace roq {
namespace utils {

template <typename T, typename U>
inline std::tuple<date::year_month_day, date::hh_mm_ss<T>> split(U value) {
  date::sys_days date{std::chrono::duration_cast<date::days>(value)};
  date::hh_mm_ss<T> time{
      std::chrono::duration_cast<T>(value % std::chrono::duration_cast<U>(date::days{1}))};
  return {date, time};
}

}  // namespace utils
}  // namespace roq
