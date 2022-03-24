/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <utility>

#include "roq/trace_info.hpp"

namespace roq {

template <typename T>
struct Trace final {
  Trace(const TraceInfo &trace_info, const T &value) : trace_info(trace_info), value(value) {}

  Trace(const Trace &) = delete;

  operator const TraceInfo &() const { return trace_info; }
  operator const T &() const { return value; }

  operator std::pair<const TraceInfo &, const T &>() const { return {trace_info, value}; }

  const TraceInfo &trace_info;
  const T &value;
};

template <typename Handler, typename T, typename... Args>
inline void create_trace_and_dispatch(Handler &&handler, const TraceInfo &trace_info, const T &value, Args &&...args) {
  Trace trace(trace_info, value);
  handler(trace, std::forward<Args>(args)...);
}

}  // namespace roq
