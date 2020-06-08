/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <cstdint>
#include <string_view>

#include "roq/compat.h"

namespace roq {
namespace metrics {

class ROQ_PUBLIC Writer {
 public:
  virtual Writer& write_type(
      const std::string_view& name,
      const std::string_view& type) = 0;
  // counter + gauge
  virtual Writer& write_simple(
      const std::string_view& name,
      const std::string_view& labels,
      uint64_t value) = 0;
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
  Writer& write(
      const T& collector,
      const std::string_view& name) {
    collector.write(*this, name);
    return *this;
  }
};

}  // namespace metrics
}  // namespace roq
