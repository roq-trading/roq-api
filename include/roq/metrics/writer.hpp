/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/compat.hpp"

#include <cstdint>
#include <string_view>

#include "roq/metrics/type.hpp"

namespace roq {
namespace metrics {

//! Writer (Prometheus' exposition format)
struct ROQ_PUBLIC Writer {
  virtual ~Writer() {}

  //! Write the TYPE header
  virtual Writer &write_type(std::string_view const &name, std::string_view const &type) = 0;

  //! Write simple metrics (counter and gauge)
  virtual Writer &write_simple(std::string_view const &name, std::string_view const &labels, uint64_t value) = 0;

  //! Write histogram buckets
  virtual Writer &write_bucket(
      std::string_view const &name, std::string_view const &labels, double quantile, uint64_t value) = 0;

  //! Write histogram sum
  virtual Writer &write_sum(std::string_view const &name, std::string_view const &labels, double value) = 0;

  //! Write histogram count
  virtual Writer &write_count(std::string_view const &name, std::string_view const &labels, uint64_t count) = 0;

  //! Finish this metric
  virtual Writer &finish() = 0;

  //! Dispatch helper
  template <typename T, typename... Args>
  Writer &write(T const &collector, std::string_view const &name, Args &&...args) {
    collector.write(*this, name, std::forward<Args>(args)...);
    return *this;
  }
  template <typename T, typename... Args>
  Writer &write(T const &collector, Type type, Args &&...args) {
    return write(collector, get_metrics_name(type), std::forward<Args>(args)...);
  }
};

}  // namespace metrics
}  // namespace roq
