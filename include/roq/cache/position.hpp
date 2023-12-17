/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

#include "roq/utils/update.hpp"

namespace roq {
namespace cache {

struct Position final {
  Position() = default;

  Position(Position const &) = delete;
  Position(Position &&) = default;

  void clear() {
    stream_id = {};
    external_account.clear();
    for (auto &margin_mode : magic_enum::enum_values<MarginMode>()) {
      auto index = to_index(margin_mode);
      new (&items_[index]) Item{
          .long_quantity = NaN,
          .short_quantity = NaN,
          .exchange_time_utc = {},
      };
    }
  }

  [[nodiscard]] bool operator()(PositionUpdate const &position_update) {
    auto dirty = false;
    dirty |= utils::update(stream_id, position_update.stream_id);
    dirty |= utils::update_if_not_empty(external_account, position_update.external_account);
    auto index = to_index(position_update.margin_mode);
    auto &item = items_[index];
    dirty |= utils::update(item.long_quantity, position_update.long_quantity);
    dirty |= utils::update(item.short_quantity, position_update.short_quantity);
    dirty |= utils::update_if_not_empty(item.exchange_time_utc, position_update.exchange_time_utc);
    return dirty;
  }

  template <typename Context, typename Callback>
  bool dispatch(Context const &context, Callback callback) const {
    auto result = false;
    for (auto &margin_mode : magic_enum::enum_values<MarginMode>()) {
      auto index = to_index(margin_mode);
      auto &item = items_[index];
      if (std::isnan(item.long_quantity) && std::isnan(item.short_quantity))
        continue;
      result = true;
      auto position_update = PositionUpdate{
          .stream_id = stream_id,
          .account = context.account,
          .margin_mode = margin_mode,
          .exchange = context.exchange,
          .symbol = context.symbol,
          .external_account = external_account,
          .long_quantity = item.long_quantity,
          .short_quantity = item.short_quantity,
          .update_type = UpdateType::SNAPSHOT,
          .exchange_time_utc = item.exchange_time_utc,
          .sending_time_utc = {},
      };
      callback(position_update);
    }
    return result;
  }

  size_t size() const {
    auto result = size_t{};
    for (auto &margin_mode : magic_enum::enum_values<MarginMode>()) {
      auto index = to_index(margin_mode);
      auto &item = items_[index];
      if (!(std::isnan(item.long_quantity) && std::isnan(item.short_quantity)))
        ++result;
    }
    return result;
  }

  uint16_t stream_id = {};

  ExternalAccount external_account;

 private:
  struct Item final {
    double long_quantity = NaN;
    double short_quantity = NaN;
    std::chrono::nanoseconds exchange_time_utc = {};
  };

  std::array<Item, magic_enum::enum_count<MarginMode>()> items_;

  static size_t to_index(MarginMode margin_mode) {
    return static_cast<std::underlying_type<std::decay<decltype(margin_mode)>::type>::type>(margin_mode);
  }
};

}  // namespace cache
}  // namespace roq
