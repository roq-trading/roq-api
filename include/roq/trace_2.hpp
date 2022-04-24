/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <utility>

#include "roq/trace_info.hpp"

namespace roq {

template <typename T>
struct Trace2 final {
  Trace2(const TraceInfo &trace_info, T &value) : trace_info(trace_info), value(value) {}

  Trace2(const Trace2 &) = delete;

  operator const TraceInfo &() const { return trace_info; }
  operator T &() const { return value; }

  operator std::pair<const TraceInfo &, T &>() const { return {trace_info, value}; }

  const TraceInfo &trace_info;
  T &value;
};

template <typename Handler, typename T, typename... Args>
inline void create_trace_2_and_dispatch(Handler &&handler, const TraceInfo &trace_info, T &value, Args &&...args) {
  Trace2 trace(trace_info, value);
  handler(trace, std::forward<Args>(args)...);
}

}  // namespace roq
