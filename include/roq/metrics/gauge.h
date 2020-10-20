/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <atomic>
#include <string>
#include <string_view>

#include "roq/compat.h"

#include "roq/metrics/writer.h"

namespace roq {
namespace metrics {

//! Gauge
template <typename T>
class alignas(ROQ_CACHELINE_SIZE) Gauge {
 public:
  Gauge() = default;
  explicit Gauge(const std::string_view &labels) : _labels(labels) {}

  Gauge(const Gauge &) = delete;
  Gauge(Gauge &&) = delete;

  //! Set gauge to specific value
  void set(T value) { _data.value.store(value, std::memory_order_release); }

  //! Write formatted output
  void write(Writer &writer, const std::string_view &name) const {
    return write(writer, name, _labels);
  }

  //! Write formatted output
  void write(
      Writer &writer,
      const std::string_view &name,
      const std::string_view &labels) const {
    auto value = _data.value.load(std::memory_order_acquire);
    writer.write_type(name, "gauge").write_simple(name, labels, value).finish();
  }

 private:
  struct alignas(ROQ_CACHELINE_SIZE) Data final {
    std::atomic<T> value = { 0 };
  } _data;
  const std::string _labels;
  // assumptions
  static_assert(sizeof(Data) == ROQ_CACHELINE_SIZE);
  static_assert(sizeof(std::atomic<T>) == sizeof(T));
  static_assert(std::alignment_of_v<std::atomic<T> > == std::alignment_of_v<T>);
};

}  // namespace metrics
}  // namespace roq
