/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <utility>

#include "roq/trace_info.hpp"

namespace roq {

template <typename T>
struct Trace final {
  using value_type = T;

  Trace(const TraceInfo &trace_info, value_type &value) : trace_info(trace_info), value(value) {}

  Trace(const Trace &) = delete;

  operator const TraceInfo &() const { return trace_info; }
  operator value_type &() const { return value; }

  operator std::pair<const TraceInfo &, T &>() const { return {trace_info, value}; }

  const TraceInfo &trace_info;
  value_type &value;
};

template <typename Handler, typename T, typename... Args>
inline void create_trace_and_dispatch(Handler &&handler, const TraceInfo &trace_info, T &value, Args &&...args) {
  const Trace event{trace_info, value};
  handler(event, std::forward<Args>(args)...);
}

}  // namespace roq
