/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/metrics.h"

using namespace roq;  // NOLINT

namespace {
class Writer : public Metrics {
 protected:
  Metrics& write_type(
      const std::string_view& name,
      const std::string_view& type) override {
    return *this;
  }
  Metrics& write_simple(
      const std::string_view& name,
      const std::string_view& labels,
      double value) override {
    return *this;
  }
  Metrics& write_bucket(
      const std::string_view& name,
      const std::string_view& labels,
      double quantile,
      uint64_t value) override {
    return *this;
  }
  Metrics& write_sum(
      const std::string_view& name,
      const std::string_view& labels,
      double value) override {
    return *this;
  }
  Metrics& write_count(
      const std::string_view& name,
      const std::string_view& labels,
      uint64_t count) override {
    return *this;
  }
  Metrics& finish() override {
    return *this;
  }
};
}  // namespace

TEST(histogram, simple) {
  Histogram<1, 2, 3, 4, 5, 6> histogram("test");
  histogram.update(3);
  Writer writer;
  writer.write(histogram);
}
