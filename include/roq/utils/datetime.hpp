/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <cassert>
#include <chrono>

namespace roq {
namespace utils {

// wrapper class to format std::chrono durations as iso 8601

template <typename T>
struct DateTime_iso8601 final {
  explicit DateTime_iso8601(T const &value) : value_{value} {}

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
  T const value_;
};

template <typename T>
struct DateTime_rfc2616 final {
  explicit DateTime_rfc2616(T const &value) : value_{value} {}

  template <typename Context>
  auto format_to(Context &context) const {
    using namespace std::literals;
    std::chrono::sys_days days{std::chrono::duration_cast<std::chrono::days>(value_)};
    std::chrono::year_month_day ymd{days};
    auto weekday = [&]() -> std::string_view {
      std::chrono::weekday weekday{days};
      switch (weekday.c_encoding()) {
        case std::chrono::Sunday.c_encoding():
          return "Sun"sv;
        case std::chrono::Monday.c_encoding():
          return "Mon"sv;
        case std::chrono::Tuesday.c_encoding():
          return "Tue"sv;
        case std::chrono::Wednesday.c_encoding():
          return "Wed"sv;
        case std::chrono::Thursday.c_encoding():
          return "Thu"sv;
        case std::chrono::Friday.c_encoding():
          return "Fri"sv;
        case std::chrono::Saturday.c_encoding():
          return "Sat"sv;
        default:
          assert(false);
          return {};
      }
    }();
    auto month = [&]() -> std::string_view {
      std::chrono::month month{ymd.month()};
      switch (static_cast<unsigned>(month)) {
        case static_cast<unsigned>(std::chrono::January):
          return "Jan"sv;
        case static_cast<unsigned>(std::chrono::February):
          return "Feb"sv;
        case static_cast<unsigned>(std::chrono::March):
          return "Mar"sv;
        case static_cast<unsigned>(std::chrono::April):
          return "Apr"sv;
        case static_cast<unsigned>(std::chrono::May):
          return "May"sv;
        case static_cast<unsigned>(std::chrono::June):
          return "Jun"sv;
        case static_cast<unsigned>(std::chrono::July):
          return "Jul"sv;
        case static_cast<unsigned>(std::chrono::August):
          return "Aug"sv;
        case static_cast<unsigned>(std::chrono::September):
          return "Sep"sv;
        case static_cast<unsigned>(std::chrono::October):
          return "Oct"sv;
        case static_cast<unsigned>(std::chrono::November):
          return "Nov"sv;
        case static_cast<unsigned>(std::chrono::December):
          return "Dec"sv;
        default:
          assert(false);
          return {};
      }
    }();
    auto tmp = std::chrono::time_point_cast<T>(days);
    auto tmp2 = std::chrono::duration_cast<T>(tmp.time_since_epoch());
    auto tmp3 = value_ - tmp2;
    std::chrono::hh_mm_ss hms{tmp3};
    return fmt::format_to(
        context.out(),
        "{}, {:02} {} {:04} {:02}:{:02}:{:02} GMT"sv,
        weekday,
        static_cast<unsigned>(ymd.day()),
        month,
        static_cast<int>(ymd.year()),
        hms.hours().count(),
        hms.minutes().count(),
        hms.seconds().count());
  }

 private:
  T const value_;
};
}  // namespace utils
}  // namespace roq

template <typename T>
struct fmt::formatter<roq::utils::DateTime_iso8601<T>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::utils::DateTime_iso8601<T> const &value, format_context &context) const {
    return value.format_to(context);
  }
};

template <typename T>
struct fmt::formatter<roq::utils::DateTime_rfc2616<T>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::utils::DateTime_rfc2616<T> const &value, format_context &context) const {
    return value.format_to(context);
  }
};
