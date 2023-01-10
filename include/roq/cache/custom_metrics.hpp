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
      : user{custom_metrics_update.user}, label{custom_metrics_update.label} {}
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
        .user = user,
        .label = label,
        .account = context.account,
        .exchange = context.exchange,
        .symbol = context.symbol,
        .measurements = {const_cast<Measurement *>(std::data(measurements)), std::size(measurements)},  // XXX const
        .update_type = UpdateType::SNAPSHOT,
    };
  }

  uint16_t stream_id = {};

  User const user;
  Label const label;
  std::vector<Measurement> measurements;

 protected:
  bool update_incremental(auto const &custom_metrics_update) {
    auto changed = false;
    for (auto &[key, value] : custom_metrics_update.measurements) {
      auto iter = lookup_.find(key);
      if (iter == std::end(lookup_)) {
        iter = lookup_.emplace(key, std::size(measurements)).first;
        measurements.push_back({key, value});
        changed = true;
      } else {
        auto index = (*iter).second;
        auto &tmp = measurements[index];
        changed |= utils::update(tmp.value, value);
      }
    }
    return changed;
  }

  void update_snapshot(auto const &custom_metrics_update) {
    measurements.clear();
    lookup_.clear();
    for (auto &[key, value] : custom_metrics_update.measurements) {
      auto iter = lookup_.emplace(key, std::size(measurements)).first;
      measurements.push_back({key, value});
    }
  }

 private:
  absl::node_hash_map<MeasurementKey, size_t> lookup_;
};

}  // namespace cache
}  // namespace roq
