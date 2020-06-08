/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <atomic>
#include <string>
#include <string_view>

#include "roq/compat.h"
#include "roq/platform.h"

#include "roq/metrics/writer.h"

namespace roq {
namespace metrics {

template <typename T>
class alignas(cache_line_size()) Gauge {
 public:
  Gauge() = default;
  explicit Gauge(const std::string_view& labels)
      : _labels(labels) {
  }

  Gauge(const Gauge&) = delete;
  Gauge(Gauge&&) = delete;

  void set(T value) {
    _data.value.store(
        value,
        std::memory_order_release);
  }

  void write(
      Writer& writer,
      const std::string_view& name) const {
    return write(
        writer,
        name,
        _labels);
  }

  void write(
      Writer& writer,
      const std::string_view& name,
      const std::string_view& labels) const {
    auto value = _data.value.load(std::memory_order_acquire);
    writer
      .write_type(name, "gauge")
      .write_simple(name, labels, value)
      .finish();
  }

 private:
  struct alignas(cache_line_size()) Data final {
    std::atomic<T> value = {0};
  } _data;
  // assumptions
  static_assert(sizeof(Data) == cache_line_size());
  static_assert(sizeof(std::atomic<T>) == sizeof(T));
  static_assert(std::alignment_of_v<std::atomic<T> > == std::alignment_of_v<T>);
  // XXX remove
  const std::string _name;
  const std::string _labels;
};

}  // namespace metrics
}  // namespace roq
