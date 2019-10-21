/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/metrics.h"

using namespace roq;  // NOLINT

namespace {
class Writer : public Metrics {
 protected:
  Metrics& write_type(
      const std::string_view&,
      const std::string_view&) override {
    return *this;
  }
  Metrics& write_simple(
      const std::string_view&,
      const std::string_view&,
      uint64_t) override {
    return *this;
  }
  Metrics& write_bucket(
      const std::string_view&,
      const std::string_view&,
      double,
      uint64_t) override {
    return *this;
  }
  Metrics& write_sum(
      const std::string_view&,
      const std::string_view&,
      double) override {
    return *this;
  }
  Metrics& write_count(
      const std::string_view&,
      const std::string_view&,
      uint64_t) override {
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
