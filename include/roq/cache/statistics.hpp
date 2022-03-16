/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <array>
#include <limits>

#include "roq/api.hpp"

#include "roq/utils/update.hpp"

namespace roq {
namespace cache {

class Statistics final {
 public:
  Statistics(const std::string_view &exchange, const std::string_view &symbol) : exchange(exchange), symbol(symbol) {
    clear();  // note! initialize array
  }

  Statistics(const Statistics &) = delete;
  Statistics(Statistics &&) = default;

  void clear() {
    stream_id = {};
    for (auto &type : StatisticsType::values()) {
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
  bool update(const StatisticsUpdate &statistics_update, Storage &storage, Callback callback) {
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
        storage.emplace_back(roq::Statistics{
            .type = tmp.type,
            .value = tmp.value,
            .begin_time_utc = tmp.begin_time_utc,
            .end_time_utc = tmp.end_time_utc,
        });
      }
    }
    if (dirty) {
      stream_id = statistics_update.stream_id;
      exchange_time_utc = statistics_update.exchange_time_utc;
      roq::StatisticsUpdate result{
          .stream_id = stream_id,
          .exchange = exchange,
          .symbol = symbol,
          .statistics = storage,
          .update_type = UpdateType::INCREMENTAL,  // note!
          .exchange_time_utc = {},
      };
      callback(result);
    }
    return dirty;
  }

  [[nodiscard]] bool operator()(const StatisticsUpdate &statistics_update) {
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

  [[nodiscard]] bool operator()(const Event<StatisticsUpdate> &event) { return (*this)(event.value); }

  // note! this will include *all* statistics (whether empty or not)
  // use the extract method if you only care about non-empty statistics
  [[nodiscard]] operator roq::StatisticsUpdate() {
    return {
        .stream_id = stream_id,
        .exchange = exchange,
        .symbol = symbol,
        .statistics = statistics,             // XXX reason for non-const method
        .update_type = UpdateType::SNAPSHOT,  // note!
        .exchange_time_utc = exchange_time_utc,
    };
  }

  // note!
  // a storage container must be provided for storing *non-empty* statistics
  // the storage interface should support the emplace_back() method
  template <typename Storage>
  [[nodiscard]] roq::StatisticsUpdate extract(Storage &storage) const {
    for (auto &type : StatisticsType::values()) {
      if (type == StatisticsType{})  // skip undefined
        continue;
      auto index = to_index(type);
      auto &tmp = statistics[index];
      if (!is_empty(tmp))
        storage.emplace_back(roq::Statistics{
            .type = tmp.type,
            .value = tmp.value,
            .begin_time_utc = tmp.begin_time_utc,
            .end_time_utc = tmp.end_time_utc,
        });
    }
    return {
        .stream_id = stream_id,
        .exchange = exchange,
        .symbol = symbol,
        .statistics = storage,
        .update_type = UpdateType::SNAPSHOT,  // note!
        .exchange_time_utc = exchange_time_utc,
    };
  }

  // note! a callback for each non-empty statistics
  template <typename Callback>
  void dispatch(Callback callback) const {
    for (auto &type : StatisticsType::values()) {
      auto index = to_index(type);
      auto &tmp = statistics[index];
      if (!is_empty(tmp))
        callback(tmp);
    }
  }

  uint16_t stream_id = {};
  string<MAX_LENGTH_EXCHANGE> exchange;
  string<MAX_LENGTH_SYMBOL> symbol;
  std::array<roq::Statistics, StatisticsType::count()> statistics;
  std::chrono::nanoseconds exchange_time_utc = {};

 private:
  static size_t to_index(StatisticsType type) {
    return static_cast<std::underlying_type<std::decay<decltype(type)>::type>::type>(type);
  }
  static bool is_empty(const roq::Statistics &statistics) { return std::isnan(statistics.value); }
};

}  // namespace cache
}  // namespace roq
