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

template <typename T>
class alignas(ROQ_CACHELINE_SIZE) Counter {
 public:
  Counter() = default;
  explicit Counter(const std::string_view& labels)
      : _labels(labels) {
  }

  Counter(const Counter&) = delete;
  Counter(Counter&&) = delete;

  Counter& operator++() {
    _data.value.fetch_add(
        1,
        std::memory_order_release);
    return *this;
  }

  void update(uint64_t value) noexcept {
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
      .write_type(name, "counter")
      .write_simple(name, labels, value)
      .finish();
  }

 private:
  struct alignas(ROQ_CACHELINE_SIZE) Data final {
    std::atomic<T> value = {0};
  } _data;
  const std::string _labels;
  // assumptions
  static_assert(sizeof(Data) == ROQ_CACHELINE_SIZE);
  static_assert(sizeof(std::atomic<T>) == sizeof(T));
  static_assert(std::alignment_of_v<std::atomic<T> > == std::alignment_of_v<T>);
};

}  // namespace metrics
}  // namespace roq
