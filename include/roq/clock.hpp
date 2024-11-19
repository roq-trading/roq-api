/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <ctime>

#include "roq/exceptions.hpp"

namespace roq {

namespace detail {
template <clockid_t clock>
inline auto get_time_helper() {
  struct timespec time;
  auto res = ::clock_gettime(clock, &time);
  if (res != 0) [[unlikely]] {
    using namespace std::literals;
    throw SystemError{std::error_code{errno, std::system_category()}, "clock_gettime"sv};
  }
  return std::chrono::nanoseconds{static_cast<int64_t>(time.tv_sec) * UINT64_C(1000000000) + static_cast<int64_t>(time.tv_nsec)};
}
}  // namespace detail

namespace clock {
template <typename T = std::chrono::nanoseconds>
inline T get_system() {
  return std::chrono::duration_cast<T>(detail::get_time_helper<CLOCK_MONOTONIC>());
}

template <typename T = std::chrono::nanoseconds>
inline T get_realtime() {
  return std::chrono::duration_cast<T>(detail::get_time_helper<CLOCK_REALTIME>());
}
}  // namespace clock

}  // namespace roq
