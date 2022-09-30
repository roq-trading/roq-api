/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/compat.hpp"

#include <atomic>
#include <cstdint>
#include <limits>
#include <string>
#include <string_view>

#include "roq/metrics/base.hpp"
#include "roq/metrics/writer.hpp"

namespace roq {
namespace metrics {

//! Histogram
template <uint64_t N0, uint64_t N1, uint64_t N2, uint64_t N3, uint64_t N4, uint64_t N5>
struct alignas(ROQ_CACHELINE_SIZE) Histogram : public Base {
 public:
  constexpr uint64_t threshold() const { return N5; }

  Histogram() = default;
  explicit Histogram(std::string_view const &labels) : labels_(labels) {}
  Histogram(std::string_view const &label_name_0, std::string_view const &label_value_0)
      : labels_(create_labels(label_name_0, label_value_0)) {}
  Histogram(
      std::string_view const &label_name_0,
      std::string_view const &label_value_0,
      std::string_view const &label_name_1,
      std::string_view const &label_value_1)
      : labels_(create_labels(label_name_0, label_value_0, label_name_1, label_value_1)) {}
  Histogram(
      std::string_view const &label_name_0,
      std::string_view const &label_value_0,
      std::string_view const &label_name_1,
      std::string_view const &label_value_1,
      std::string_view const &label_name_2,
      std::string_view const &label_value_2)
      : labels_(create_labels(label_name_0, label_value_0, label_name_1, label_value_1, label_name_2, label_value_2)) {}

  Histogram(Histogram const &) = delete;
  Histogram(Histogram &&) = delete;

  //! Prefetch data into L1 cache
  inline void prefetch() noexcept { __builtin_prefetch(&data_, 1, 3); }

  //! Update histogram with specific value
  inline bool update(uint64_t value) noexcept {
    bool result = false;
    if (value < N0) {
      data_.bucket_0 += 1;
    } else if (value < N1) {
      data_.bucket_1 += 1;
    } else if (value < N2) {
      data_.bucket_2 += 1;
    } else if (value < N3) {
      data_.bucket_3 += 1;
    } else if (value < N4) {
      data_.bucket_4 += 1;
    } else if (value < N5) {
      data_.bucket_5 += 1;
    } else {
      data_.bucket_6 += 1;
      result = true;
    }
    data_.sum.fetch_add(value, std::memory_order_release);
    return result;
  }

  //! Write formatted output
  Writer &write(Writer &writer, std::string_view const &name) const { return write(writer, name, labels_); }

  //! Write formatted output
  Writer &write(Writer &writer, std::string_view const &name, std::string_view const &labels) const {
    using namespace std::literals;
    auto sum = data_.sum.load(std::memory_order_acquire);
    auto bucket_0 = data_.bucket_0;
    auto bucket_1 = bucket_0 + data_.bucket_1;
    auto bucket_2 = bucket_1 + data_.bucket_2;
    auto bucket_3 = bucket_2 + data_.bucket_3;
    auto bucket_4 = bucket_3 + data_.bucket_4;
    auto bucket_5 = bucket_4 + data_.bucket_5;
    auto bucket_6 = bucket_5 + data_.bucket_6;
    writer
        .write_type(name, "histogram"sv)  //
        .write_bucket(name, labels, N0, bucket_0)
        .write_bucket(name, labels, N1, bucket_1)
        .write_bucket(name, labels, N2, bucket_2)
        .write_bucket(name, labels, N3, bucket_3)
        .write_bucket(name, labels, N4, bucket_4)
        .write_bucket(name, labels, N5, bucket_5)
        .write_bucket(name, labels, std::numeric_limits<double>::infinity(), bucket_6)
        .write_sum(name, labels, sum)
        .write_count(name, labels, bucket_6)
        .finish();
    return writer;
  }

 private:
  struct alignas(ROQ_CACHELINE_SIZE) Data {
    std::atomic<uint64_t> sum = {0};
    uint64_t bucket_0;
    uint64_t bucket_1;
    uint64_t bucket_2;
    uint64_t bucket_3;
    uint64_t bucket_4;
    uint64_t bucket_5;
    uint64_t bucket_6;
  } data_ = {};
  const std::string labels_;
  // assumptions
  static_assert(sizeof(Data) == ROQ_CACHELINE_SIZE);
  static_assert(sizeof(std::atomic<uint64_t>) == sizeof(uint64_t));
  static_assert(std::alignment_of_v<std::atomic<uint64_t> > == std::alignment_of_v<uint64_t>);
};

// convenience

using internal_latency_t = Histogram<500, 1000, 2000, 5000, 10000, 20000>;

using external_latency_t = Histogram<10000, 100000, 1000000, 10000000, 100000000, 1000000000>;

}  // namespace metrics
}  // namespace roq
