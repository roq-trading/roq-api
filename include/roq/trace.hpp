/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <type_traits>
#include <utility>

#include "roq/name.hpp"
#include "roq/trace_info.hpp"

namespace roq {

template <typename T>
struct Trace final {
  using value_type = std::remove_cvref_t<T>;

  Trace(TraceInfo const &trace_info, T const &value) : trace_info{trace_info}, value{value} {}

  Trace(Trace &&) = delete;
  Trace(Trace const &) = delete;

  void operator=(Trace &&) = delete;
  void operator=(Trace const &) = delete;

  operator TraceInfo const &() const { return trace_info; }
  operator value_type const &() const { return value; }

  operator std::pair<TraceInfo const &, T const &>() const { return {trace_info, value}; }

  TraceInfo const &trace_info;
  value_type const &value;

  template <typename... Args>
  static void create_and_dispatch(auto &handler, TraceInfo const &trace_info, T const &value, Args &&...args) {
    Trace const event{trace_info, value};
    handler(event, std::forward<Args>(args)...);
  }
};

template <typename T, typename... Args>
inline void create_trace_and_dispatch(auto &handler, TraceInfo const &trace_info, T const &value, Args &&...args) {
  Trace<T>::create_and_dispatch(handler, trace_info, value, std::forward<Args>(args)...);
}

}  // namespace roq

template <typename T>
struct fmt::formatter<roq::Trace<T>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<T> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"({}={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        roq::get_name<T>(),
        event.value,
        event.trace_info);
  }
};
