/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <cstdint>

#include <atomic>
#include <limits>
#include <string>
#include <string_view>

#include "roq/api.h"
#include "roq/builtins.h"
#include "roq/platform.h"

namespace roq {

class Metrics {
 public:
  virtual Metrics& write_type(
      const std::string_view& name,
      const std::string_view& type) = 0;
  // counter + gauge
  virtual Metrics& write_simple(
      const std::string_view& name,
      const std::string_view& labels,
      uint64_t value) = 0;
  // histogram
  virtual Metrics& write_bucket(
      const std::string_view& name,
      const std::string_view& labels,
      double quantile,
      uint64_t value) = 0;
  virtual Metrics& write_sum(
      const std::string_view& name,
      const std::string_view& labels,
      double value) = 0;
  virtual Metrics& write_count(
      const std::string_view& name,
      const std::string_view& labels,
      uint64_t count) = 0;
  // finish
  virtual Metrics& finish() = 0;
  // utilities
  template <typename T>
  Metrics& write(const T& collector) {
    collector.write(*this);
    return *this;
  }
};

struct MetricsBase {
  static std::string create_labels(
      const std::string_view& label_name_0,
      const std::string_view& label_value_0);
  static std::string create_labels(
      const std::string_view& label_name_0,
      const std::string_view& label_value_0,
      const std::string_view& label_name_1,
      const std::string_view& label_value_1);
  static std::string create_labels(
      const std::string_view& label_name_0,
      const std::string_view& label_value_0,
      const std::string_view& label_name_1,
      const std::string_view& label_value_1,
      const std::string_view& label_name_2,
      const std::string_view& label_value_2);
};

template <uint64_t N0,
          uint64_t N1,
          uint64_t N2,
          uint64_t N3,
          uint64_t N4,
          uint64_t N5>
struct alignas(cache_line_size()) Histogram : public MetricsBase {
 public:
  constexpr uint64_t threshold() const {
    return N5;
  }

  explicit Histogram(
      const std::string_view& name)
      : _name(name) {
  }
  Histogram(
      const std::string_view& name,
      const std::string_view& labels)
      : _name(name),
        _labels(labels) {
  }
  Histogram(
      const std::string_view& name,
      const std::string_view& label_name_0,
      const std::string_view& label_value_0)
      : _name(name),
        _labels(
            create_labels(
                label_name_0,
                label_value_0)) {
  }
  Histogram(
      const std::string_view& name,
      const std::string_view& label_name_0,
      const std::string_view& label_value_0,
      const std::string_view& label_name_1,
      const std::string_view& label_value_1)
      : _name(name),
        _labels(
            create_labels(
                label_name_0,
                label_value_0,
                label_name_1,
                label_value_1)) {
  }
  Histogram(
      const std::string_view& name,
      const std::string_view& label_name_0,
      const std::string_view& label_value_0,
      const std::string_view& label_name_1,
      const std::string_view& label_value_1,
      const std::string_view& label_name_2,
      const std::string_view& label_value_2)
      : _name(name),
        _labels(
            create_labels(
                label_name_0,
                label_value_0,
                label_name_1,
                label_value_1,
                label_name_2,
                label_value_2)) {
  }

  Histogram(const Histogram&) = delete;
  Histogram(Histogram&&) = default;

  void operator=(const Histogram&) = delete;
  void operator=(Histogram&&) = delete;

  inline void prefetch() noexcept {
    __builtin_prefetch(&_data, 1, 3);
  }

  inline bool update(uint64_t value) noexcept {
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

  Metrics& write(Metrics& writer) const {
    return write(writer, _labels);
  }

  Metrics& write(
      Metrics& metrics,
      const std::string_view& labels) const {
    auto sum = atomic_acquire(_data.sum);
    auto bucket_0 = _data.bucket_0;
    auto bucket_1 = bucket_0 + _data.bucket_1;
    auto bucket_2 = bucket_1 + _data.bucket_2;
    auto bucket_3 = bucket_2 + _data.bucket_3;
    auto bucket_4 = bucket_3 + _data.bucket_4;
    auto bucket_5 = bucket_4 + _data.bucket_5;
    auto bucket_6 = bucket_5 + _data.bucket_6;
    metrics
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
    return metrics;
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
  } _data = {};
  static_assert(sizeof(Data) == cache_line_size());
  const std::string _name;
  const std::string _labels;
};

template <typename T>
class alignas(cache_line_size()) Counter {
 public:
  Counter(
      const std::string_view& name,
      const std::string_view& labels)
      : _data{0},
        _name(name),
        _labels(labels) {
  }

  Counter(const Counter&) = delete;
  Counter(Counter&&) = default;

  void operator=(const Counter&) = delete;
  void operator=(Counter&&) = delete;

  Counter& operator++() {
    __atomic_fetch_add(&_data.value, 1, __ATOMIC_RELEASE);
    return *this;
  }

  void update(uint64_t value) noexcept {
    atomic_release(_data.value, value);
  }

  void write(Metrics& metrics) const {
    auto value = atomic_acquire(_data.value);
    metrics
      .write_type(_name, "counter")
      .write_simple(_name, _labels, value)
      .finish();
  }

 private:
  struct alignas(cache_line_size()) Data final {
    T value = 0;
  } _data;
  static_assert(sizeof(Data) == cache_line_size());
  const std::string _name;
  const std::string _labels;
};

template <typename T>
class alignas(cache_line_size()) Gauge {
 public:
  Gauge(
      const std::string_view& name,
      const std::string_view& labels)
      : _data{0},
        _name(name),
        _labels(labels) {
  }

  Gauge(const Gauge&) = delete;
  Gauge(Gauge&&) = default;

  void operator=(const Gauge&) = delete;
  void operator=(Gauge&&) = delete;

  void set(T value) {
    atomic_release(_data.value, value);
  }

  void write(Metrics& metrics) const {
    auto value = atomic_acquire(_data.value);
    metrics
      .write_type(_name, "gauge")
      .write_simple(_name, _labels, value)
      .finish();
  }

 private:
  struct alignas(cache_line_size()) Data final {
    T value = 0;
  } _data;
  static_assert(sizeof(Data) == cache_line_size());
  const std::string _name;
  const std::string _labels;
};

// convenience

using InternalLatency = Histogram<
    500, 1000, 2000, 5000, 10000, 20000>;

using ExternalLatency = Histogram<
    10000, 100000, 1000000, 10000000, 100000000, 1000000000>;

}  // namespace roq
