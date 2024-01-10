/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/chrono.h>
#include <fmt/compile.h>
#include <fmt/format.h>

#include <utility>

namespace roq {
namespace utils {

template <typename T, typename U>
inline std::pair<std::chrono::year_month_day, std::chrono::hh_mm_ss<T>> split(U value) {
  std::chrono::sys_days date{std::chrono::duration_cast<std::chrono::days>(value)};
  std::chrono::hh_mm_ss<T> time{
      std::chrono::duration_cast<T>(value % std::chrono::duration_cast<U>(std::chrono::days{1}))};
  return {date, time};
}

}  // namespace utils
}  // namespace roq

template <>
struct fmt::formatter<std::chrono::year_month_day> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(std::chrono::year_month_day const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(year={}, )"
        R"(month={}, )"
        R"(day={})"
        R"(}})"sv,
        static_cast<int>(value.year()),
        static_cast<unsigned>(value.month()),
        static_cast<unsigned>(value.day()));
  }
};

template <>
struct fmt::formatter<std::chrono::hh_mm_ss<std::chrono::milliseconds>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(std::chrono::hh_mm_ss<std::chrono::milliseconds> const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(hours={}, )"
        R"(minutes={}, )"
        R"(seconds={} )"
        R"(milliseconds={})"
        R"(}})"sv,
        value.hours().count(),
        value.minutes().count(),
        value.seconds().count(),
        value.subseconds().count());
  }
};
