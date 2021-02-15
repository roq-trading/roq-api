/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <atomic>
#include <string>
#include <string_view>

#include "roq/compat.h"
#include "roq/literals.h"

#include "roq/metrics/writer.h"

namespace roq {
namespace metrics {

//! Counter
template <typename T>
class alignas(ROQ_CACHELINE_SIZE) Counter {
 public:
  Counter() = default;
  explicit Counter(const std::string_view &labels) : labels_(labels) {}

  Counter(const Counter &) = delete;
  Counter(Counter &&) = delete;

  //! Increment counter
  Counter &operator++() {
    data_.value.fetch_add(1, std::memory_order_release);
    return *this;
  }

  //! Increment counter by value
  Counter &operator+=(uint64_t value) {
    data_.value.fetch_add(value, std::memory_order_release);
    return *this;
  }

  //! Update counter to specific value
  void update(uint64_t value) noexcept { data_.value.store(value, std::memory_order_release); }

  //! Write formatted output
  void write(Writer &writer, const std::string_view &name) const {
    return write(writer, name, labels_);
  }

  //! Write formatted output
  void write(Writer &writer, const std::string_view &name, const std::string_view &labels) const {
    using namespace roq::literals;
    auto value = data_.value.load(std::memory_order_acquire);
    writer  //
        .write_type(name, "counter"_sv)
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
