/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <utility>

#include "roq/trace_info.hpp"

namespace roq {

template <typename T>
struct Trace final {
  using value_type = T;

  Trace(TraceInfo const &trace_info_, value_type &value_) : trace_info(trace_info_), value(value_) {}

  Trace(Trace const &) = delete;

  operator TraceInfo const &() const { return trace_info; }
  operator value_type &() const { return value; }

  operator std::pair<TraceInfo const &, T &>() const { return {trace_info, value}; }

  TraceInfo const &trace_info;
  value_type &value;
};

template <typename Handler, typename T, typename... Args>
inline void create_trace_and_dispatch(Handler &&handler, TraceInfo const &trace_info, T &value, Args &&...args) {
  const Trace event{trace_info, value};
  handler(event, std::forward<Args>(args)...);
}

}  // namespace roq
