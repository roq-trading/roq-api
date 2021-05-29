/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

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
  explicit Gauge(const std::string_view &labels) : labels_(labels) {}

  Gauge(const Gauge &) = delete;
  Gauge(Gauge &&) = delete;

  //! Set gauge to specific value
  void set(T value) { data_.value.store(value, std::memory_order_release); }

  //! Write formatted output
  void write(Writer &writer, const std::string_view &name) const { return write(writer, name, labels_); }

  //! Write formatted output
  void write(Writer &writer, const std::string_view &name, const std::string_view &labels) const {
    using namespace roq::literals;
    auto value = data_.value.load(std::memory_order_acquire);
    writer  //
        .write_type(name, "gauge"_sv)
        .write_simple(name, labels, value)
        .finish();
  }

 private:
  struct alignas(ROQ_CACHELINE_SIZE) Data final {
    std::atomic<T> value = {0};
  } data_;
  const std::string labels_;
  // assumptions
  static_assert(sizeof(Data) == ROQ_CACHELINE_SIZE);
  static_assert(sizeof(std::atomic<T>) == sizeof(T));
  static_assert(std::alignment_of_v<std::atomic<T> > == std::alignment_of_v<T>);
};

}  // namespace metrics
}  // namespace roq
