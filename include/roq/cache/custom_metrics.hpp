/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <absl/container/node_hash_map.h>

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
  explicit CustomMetrics(CustomMetricsUpdate const &custom_metrics_update)
      : label{custom_metrics_update.label}, sending_time_utc{custom_metrics_update.sending_time_utc},
        user{custom_metrics_update.user} {}
  explicit CustomMetrics(roq::CustomMetrics const &custom_metrics) : label{custom_metrics.label} {}

  CustomMetrics(CustomMetrics const &) = delete;
  CustomMetrics(CustomMetrics &&) = default;

  [[nodiscard]] bool operator()(CustomMetricsUpdate const &custom_metrics_update) {
    switch (custom_metrics_update.update_type) {
      using enum UpdateType;
      case UNDEFINED:
      case INCREMENTAL:
        return update_incremental(custom_metrics_update);
      case SNAPSHOT:
        update_snapshot(custom_metrics_update);
        return true;
      case STALE:
        assert(false);  // not supported
        break;
    }
    return false;
  }

  [[nodiscard]] bool operator()(roq::CustomMetrics const &custom_metrics) {
    switch (custom_metrics.update_type) {
      using enum UpdateType;
      case UNDEFINED:
      case INCREMENTAL:
        return update_incremental(custom_metrics);
      case SNAPSHOT:
        update_snapshot(custom_metrics);
        return true;
      case STALE:
        assert(false);  // not supported
        break;
    }
    return false;
  }

  [[nodiscard]] bool operator()(Event<CustomMetricsUpdate> const &event) { return (*this)(event.value); }

  template <typename Context>
  [[nodiscard]] CustomMetricsUpdate convert(Context const &context) const {
    return {
        .label = label,
        .account = context.account,
        .exchange = context.exchange,
        .symbol = context.symbol,
        .measurements = measurements,
        .update_type = UpdateType::SNAPSHOT,
        .sending_time_utc = sending_time_utc,
        .user = user,
    };
  }

  uint16_t stream_id = {};

  Label const label;
  std::vector<Measurement> measurements;
  std::chrono::nanoseconds sending_time_utc = {};
  User const user;

 protected:
  template <typename T>
  bool update_incremental(T const &custom_metrics) {
    auto changed = false;
    for (auto &[key, value] : custom_metrics.measurements) {
      auto iter = lookup_.find(key);
      if (iter == std::end(lookup_)) {
        lookup_.emplace(key, std::size(measurements));
        measurements.push_back({key, value});
        changed = true;
      } else {
        auto index = (*iter).second;
        auto &tmp = measurements[index];
        changed |= utils::update(tmp.value, value);
      }
    }
    if constexpr (std::is_same<T, CustomMetricsUpdate>::value) {
      changed |= utils::update(sending_time_utc, custom_metrics.sending_time_utc);
    }
    return changed;
  }

  void update_snapshot(auto const &custom_metrics_update) {
    measurements.clear();
    lookup_.clear();
    for (auto &[key, value] : custom_metrics_update.measurements) {
      lookup_.emplace(key, std::size(measurements));
      measurements.push_back({key, value});
    }
  }

 private:
  absl::node_hash_map<MeasurementKey, size_t> lookup_;
};

}  // namespace cache
}  // namespace roq
