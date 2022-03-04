/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include <catch2/catch.hpp>

#include "roq/metrics.h"

using namespace roq;
using namespace std::literals;

namespace {
class Writer : public metrics::Writer {
 protected:
  metrics::Writer &write_type(const std::string_view &, const std::string_view &) override { return *this; }
  metrics::Writer &write_simple(const std::string_view &, const std::string_view &, uint64_t) override { return *this; }
  metrics::Writer &write_bucket(const std::string_view &, const std::string_view &, double, uint64_t) override {
    return *this;
  }
  metrics::Writer &write_sum(const std::string_view &, const std::string_view &, double) override { return *this; }
  metrics::Writer &write_count(const std::string_view &, const std::string_view &, uint64_t) override { return *this; }
  metrics::Writer &finish() override { return *this; }
};
}  // namespace

TEST_CASE("histogram_simple", "[histogram]") {
  metrics::Histogram<1, 2, 3, 4, 5, 6> histogram;
  histogram.update(3);
  Writer writer;
  writer.write(histogram, "test"sv);
}
