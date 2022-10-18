/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/compat.hpp"

#include <cstdint>
#include <string_view>

namespace roq {
namespace metrics {

//! Writer (Prometheus' exposition format)
class ROQ_PUBLIC Writer {
 public:
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
  template <typename T>
  Writer &write(T const &collector, std::string_view const &name) {
    collector.write(*this, name);
    return *this;
  }
};

}  // namespace metrics
}  // namespace roq
