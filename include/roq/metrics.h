/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <cstdint>
#include <limits>
#include <string>
#include <string_view>

#include "roq/platform.h"

namespace roq {
namespace metrics {

class Writer {
 public:
  virtual Writer& write_type(
      const std::string_view& name,
      const std::string_view& type) = 0;
  // counter + gauge
  virtual Writer& write_simple(
      const std::string_view& name,
      const std::string_view& labels,
      double value) = 0;
  // histogram
  virtual Writer& write_bucket(
      const std::string_view& name,
      const std::string_view& labels,
      double quantile,
      uint64_t value) = 0;
  virtual Writer& write_sum(
      const std::string_view& name,
      const std::string_view& labels,
      double value) = 0;
  virtual Writer& write_count(
      const std::string_view& name,
      const std::string_view& labels,
      uint64_t count) = 0;
  // finish
  virtual Writer& finish() = 0;
  // utilities
  template <typename T>
  Writer& write(const T& collector) {
    return collector.write(*this);
  }
};

template <uint64_t N0,
          uint64_t N1,
          uint64_t N2,
          uint64_t N3,
          uint64_t N4,
          uint64_t N5>
struct alignas(cache_line_size()) Histogram : NonCopyable {
 public:
  constexpr uint64_t threshold() const {
    return N5;
  }

  explicit Histogram(const std::string& name)
      : _name(name),
        _data{0} {
  }
  Histogram(const std::string& name, const std::string& labels)
      : _name(name),
        _labels(labels),
        _data{0} {
  }

  inline void prefetch() {
    __builtin_prefetch(&_data, 1, 3);
  }

  inline bool update(uint64_t value) {
    bool result = false;
    if (value < N0) {
      _data.bucket_0 += 1;
    } else if (value < N1) {
      _data.bucket_1 += 1;
    } else if (value < N2) {
      _data.bucket_2 += 1;
    } else if (value < N3) {
      _data.bucket_3 += 1;
    } else if (value < N4) {
      _data.bucket_4 += 1;
    } else if (value < N5) {
      _data.bucket_5 += 1;
    } else {
      _data.bucket_6 += 1;
      result = true;
    }
    __atomic_fetch_add(&_data.sum, value, __ATOMIC_RELEASE);
    return result;
  }

  Writer& write(Writer& writer) const {
    return write(writer, _labels);
  }

  Writer& write(
      Writer& writer,
      const std::string_view& labels) const {
    auto sum = __atomic_load_8(&_data.sum, __ATOMIC_ACQUIRE);
    auto bucket_0 = _data.bucket_0;
    auto bucket_1 = bucket_0 + _data.bucket_1;
    auto bucket_2 = bucket_1 + _data.bucket_2;
    auto bucket_3 = bucket_2 + _data.bucket_3;
    auto bucket_4 = bucket_3 + _data.bucket_4;
    auto bucket_5 = bucket_4 + _data.bucket_5;
    auto bucket_6 = bucket_5 + _data.bucket_6;
    return writer
      .write_type(_name, "histogram")
      .write_bucket(_name, labels, N0, bucket_0)
      .write_bucket(_name, labels, N1, bucket_1)
      .write_bucket(_name, labels, N2, bucket_2)
      .write_bucket(_name, labels, N3, bucket_3)
      .write_bucket(_name, labels, N4, bucket_4)
      .write_bucket(_name, labels, N5, bucket_5)
      .write_bucket(_name, labels,
          std::numeric_limits<double>::infinity(), bucket_6)
      .write_sum(_name, labels, sum)
      .write_count(_name, labels, bucket_6)
      .finish();
  }

 private:
  struct alignas(cache_line_size()) Data {
    uint64_t sum;
    uint64_t bucket_0;
    uint64_t bucket_1;
    uint64_t bucket_2;
    uint64_t bucket_3;
    uint64_t bucket_4;
    uint64_t bucket_5;
    uint64_t bucket_6;
  } _data;
  static_assert(sizeof(Data) == cache_line_size());
  const std::string _name;
  const std::string _labels;
};

}  // namespace metrics
}  // namespace roq
