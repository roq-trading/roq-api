/* Copyright (c) 2017-2023, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include "roq/metrics.hpp"

using namespace roq;
using namespace std::literals;

namespace {
struct Writer : public metrics::Writer {
  metrics::Writer &write_type(std::string_view const &, std::string_view const &) override { return *this; }
  metrics::Writer &write_simple(std::string_view const &, std::string_view const &, uint64_t) override { return *this; }
  metrics::Writer &write_bucket(std::string_view const &, std::string_view const &, double, uint64_t) override {
    return *this;
  }
  metrics::Writer &write_sum(std::string_view const &, std::string_view const &, double) override { return *this; }
  metrics::Writer &write_count(std::string_view const &, std::string_view const &, uint64_t) override { return *this; }
  metrics::Writer &finish() override { return *this; }
};
}  // namespace

TEST_CASE("histogram_simple", "[histogram]") {
  metrics::Histogram<1, 2, 3, 4, 5, 6> histogram;
  histogram.update(3);
  Writer writer;
  writer.write(histogram, "test"sv);
}
