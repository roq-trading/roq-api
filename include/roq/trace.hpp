/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <utility>

#include "roq/trace_info.hpp"

namespace roq {

template <typename T, typename = typename std::enable_if<!std::is_const<T>::value>::type>
struct Trace final {
  using value_type = T;

  Trace(TraceInfo const &trace_info_, T const &value_) : trace_info{trace_info_}, value{value_} {}

  Trace(Trace const &) = delete;
  Trace(Trace &&) = delete;

  void operator=(Trace const &) = delete;
  void operator=(Trace &&) = delete;

  operator TraceInfo const &() const { return trace_info; }
  operator value_type const &() const { return value; }

  operator std::pair<TraceInfo const &, T const &>() const { return {trace_info, value}; }

  TraceInfo const &trace_info;
  value_type const &value;
};

template <typename... Args>
inline void create_trace_and_dispatch(auto &handler, TraceInfo const &trace_info, auto const &value, Args &&...args) {
  Trace const event{trace_info, value};
  handler(event, std::forward<Args>(args)...);
}

}  // namespace roq
