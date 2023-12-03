/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <array>
#include <limits>

#include "roq/api.hpp"

#include "roq/utils/update.hpp"

namespace roq {
namespace cache {

struct Statistics final {
  Statistics() {
    clear();  // note! initialize array
  }

  Statistics(Statistics const &) = delete;
  Statistics(Statistics &&) = default;

  void clear() {
    stream_id = {};
    for (auto &type : magic_enum::enum_values<StatisticsType>()) {
      auto index = to_index(type);
      new (&statistics[index]) roq::Statistics{
          .type = type,  // note!
          .value = NaN,
          .begin_time_utc = {},
          .end_time_utc = {},
      };
    }
  }

  // note! (this is for server usage)
  // a storage container must be provided for storing *updated* statistics
  // the storage interface should support the emplace_back() method
  // the callback will only be called when there are updates
  template <typename Storage, typename Callback>
  bool update(StatisticsUpdate const &statistics_update, Storage &storage, Callback callback) {
    bool dirty = false;
    for (auto &iter : statistics_update.statistics) {
      auto index = to_index(iter.type);
      auto &tmp = statistics[index];
      auto changed = false;
      changed |= utils::update(tmp.value, iter.value);
      changed |= utils::update(tmp.begin_time_utc, iter.begin_time_utc);
      changed |= utils::update(tmp.end_time_utc, iter.end_time_utc);
      if (changed) {
        dirty |= changed;
        storage.push_back(tmp);
      }
    }
    if (dirty) {
      stream_id = statistics_update.stream_id;
      exchange_time_utc = statistics_update.exchange_time_utc;
      auto result = StatisticsUpdate{
          .stream_id = stream_id,
          .exchange = statistics_update.exchange,
          .symbol = statistics_update.symbol,
          .statistics = storage,
          .update_type = UpdateType::INCREMENTAL,  // note!
          .exchange_time_utc = statistics_update.exchange_time_utc,
          .exchange_sequence = statistics_update.exchange_sequence,
          .sending_time_utc = statistics_update.sending_time_utc,
      };
      callback(result);
    }
    return dirty;
  }

  [[nodiscard]] bool operator()(StatisticsUpdate const &statistics_update) {
    bool dirty = false;
    for (auto &iter : statistics_update.statistics) {
      auto index = to_index(iter.type);
      auto &tmp = statistics[index];
      dirty |= utils::update(tmp.value, iter.value);
      dirty |= utils::update(tmp.begin_time_utc, iter.begin_time_utc);
      dirty |= utils::update(tmp.end_time_utc, iter.end_time_utc);
    }
    if (dirty) {
      stream_id = statistics_update.stream_id;
      exchange_time_utc = statistics_update.exchange_time_utc;
    }
    return dirty;
  }

  [[nodiscard]] bool operator()(Event<StatisticsUpdate> const &event) { return (*this)(event.value); }

  // note! this will include *all* statistics (whether empty or not)
  // use the extract method if you only care about non-empty statistics
  template <typename Context>
  [[nodiscard]] StatisticsUpdate convert(Context const &context) const {
    return {
        .stream_id = stream_id,
        .exchange = context.exchange,
        .symbol = context.symbol,
        .statistics = statistics,
        .update_type = UpdateType::SNAPSHOT,  // note!
        .exchange_time_utc = exchange_time_utc,
        .exchange_sequence = {},
        .sending_time_utc = {},
    };
  }

  // note!
  // a storage container must be provided for storing *non-empty* statistics
  // the storage interface should support the emplace_back() method
  template <typename Context, typename Storage>
  [[nodiscard]] StatisticsUpdate extract(Context const &context, Storage &storage) const {
    for (auto &type : magic_enum::enum_values<StatisticsType>()) {
      if (type == StatisticsType{})  // skip undefined
        continue;
      auto index = to_index(type);
      auto &tmp = statistics[index];
      if (!is_empty(tmp))
        storage.push_back(tmp);
    }
    return {
        .stream_id = stream_id,
        .exchange = context.exchange,
        .symbol = context.symbol,
        .statistics = storage,
        .update_type = UpdateType::SNAPSHOT,  // note!
        .exchange_time_utc = exchange_time_utc,
        .exchange_sequence = {},
        .sending_time_utc = {},
    };
  }

  // note! a callback for each non-empty statistics
  template <typename Callback>
  void dispatch(Callback callback) const {
    for (auto &type : magic_enum::enum_values<StatisticsType>()) {
      auto index = to_index(type);
      auto &tmp = statistics[index];
      if (!is_empty(tmp))
        callback(tmp);
    }
  }

  uint16_t stream_id = {};

  std::array<roq::Statistics, magic_enum::enum_count<StatisticsType>()> statistics;
  std::chrono::nanoseconds exchange_time_utc = {};

 private:
  static size_t to_index(StatisticsType type) {
    return static_cast<std::underlying_type<std::decay<decltype(type)>::type>::type>(type);
  }
  static bool is_empty(roq::Statistics const &statistics) { return std::isnan(statistics.value); }
};

}  // namespace cache
}  // namespace roq
