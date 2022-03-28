/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <absl/container/flat_hash_map.h>

#include <array>
#include <limits>
#include <string>
#include <string_view>
#include <vector>

#include "roq/api.hpp"

#include "roq/utils/update.hpp"

namespace roq {
namespace cache {

struct CustomMetrics final {
  explicit CustomMetrics(const CustomMetricsUpdate &custom_metrics_update)
      : user(custom_metrics_update.user), label(custom_metrics_update.label) {}

  CustomMetrics(const CustomMetrics &) = delete;
  CustomMetrics(CustomMetrics &&) = default;

  [[nodiscard]] bool operator()(const CustomMetricsUpdate &custom_metrics_update) {
    for (auto &measurement : custom_metrics_update.measurements) {
      auto &[key, value] = measurement;
      auto iter = lookup_.find(key);
      if (iter == std::end(lookup_)) {
        auto res = lookup_.emplace(key, std::size(measurements));
        iter = res.first;
        measurements.push_back(measurement);
        return true;
      } else {
        auto index = (*iter).second;
        auto &tmp = measurements[index];
        auto changed = false;
        changed |= utils::update(tmp.value, measurement.value);
        return changed;
      }
    }
  }

  template <typename Context>
  [[nodiscard]] CustomMetricsUpdate convert(const Context &context) {
    return {
        .user = user,
        .label = label,
        .account = context.account,
        .exchange = context.exchange,
        .symbol = context.symbol,
        .measurements = measurements,  // XXX reason for non-const method
    };
  }

  uint16_t stream_id = {};

  const User user;
  const Label label;
  std::vector<Measurement> measurements;

 private:
  absl::flat_hash_map<MeasurementKey, size_t> lookup_;
};

}  // namespace cache
}  // namespace roq
