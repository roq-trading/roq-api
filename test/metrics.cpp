/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/metrics.h"

using namespace roq;  // NOLINT
using namespace roq::metrics;  // NOLINT

namespace {
class TestWriter : public Writer {
 protected:
  Writer& write_type(
      const std::string_view& name,
      const std::string_view& type) override {
    return *this;
  }
  Writer& write_simple(
      const std::string_view& name,
      const std::string_view& labels,
      double value) override {
    return *this;
  }
  Writer& write_bucket(
      const std::string_view& name,
      const std::string_view& labels,
      double quantile,
      uint64_t value) override {
    return *this;
  }
  Writer& write_sum(
      const std::string_view& name,
      const std::string_view& labels,
      double value) override {
    return *this;
  }
  Writer& write_count(
      const std::string_view& name,
      const std::string_view& labels,
      uint64_t count) override {
    return *this;
  }
  Writer& finish() override {
    return *this;
  }
};
}  // namespace

TEST(histogram, simple) {
  Histogram<1, 2, 3, 4, 5, 6> histogram("test");
  histogram.update(3);
  TestWriter writer;
  writer.write(histogram);
}
