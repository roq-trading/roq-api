/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

#include "roq/utils/update.hpp"

namespace roq {
namespace cache {

struct Funds final {
  Funds() = default;

  Funds(Funds const &) = delete;
  Funds(Funds &&) = default;

  void clear() {
    stream_id = {};
    external_account.clear();
    for (auto &margin_mode : magic_enum::enum_values<MarginMode>()) {
      auto index = to_index(margin_mode);
      new (&items_[index]) Item{
          .balance = NaN,
          .hold = NaN,
          .exchange_time_utc = {},
      };
    }
  }

  [[nodiscard]] bool operator()(FundsUpdate const &funds_update) {
    auto dirty = false;
    dirty |= utils::update(stream_id, funds_update.stream_id);
    dirty |= utils::update_if_not_empty(external_account, funds_update.external_account);
    auto index = to_index(funds_update.margin_mode);
    auto &item = items_[index];
    dirty |= utils::update_if_not_empty(item.balance, funds_update.balance);
    dirty |= utils::update_if_not_empty(item.hold, funds_update.hold);
    dirty |= utils::update_if_not_empty(item.exchange_time_utc, funds_update.exchange_time_utc);
    return dirty;
  }

  template <typename Context, typename Callback>
  bool dispatch(Context const &context, Callback callback) const {
    auto result = false;
    for (auto &margin_mode : magic_enum::enum_values<MarginMode>()) {
      auto index = to_index(margin_mode);
      auto &item = items_[index];
      if (std::isnan(item.balance) && std::isnan(item.hold))
        continue;
      result = true;
      auto funds_update = FundsUpdate{
          .stream_id = stream_id,
          .account = context.account,
          .currency = context.symbol,
          .margin_mode = margin_mode,
          .balance = item.balance,
          .hold = item.hold,
          .external_account = external_account,
          .update_type = UpdateType::SNAPSHOT,
          .exchange_time_utc = item.exchange_time_utc,
          .sending_time_utc = {},
      };
      callback(funds_update);
    }
    return result;
  }

  size_t size() const {
    auto result = size_t{};
    for (auto &margin_mode : magic_enum::enum_values<MarginMode>()) {
      auto index = to_index(margin_mode);
      auto &item = items_[index];
      if (!(std::isnan(item.balance) && std::isnan(item.hold)))
        ++result;
    }
    return result;
  }

  uint16_t stream_id = {};

  ExternalAccount external_account;

 private:
  struct Item final {
    double balance = NaN;
    double hold = NaN;
    std::chrono::nanoseconds exchange_time_utc = {};
  };

  std::array<Item, magic_enum::enum_count<MarginMode>()> items_;

  static size_t to_index(MarginMode margin_mode) {
    return static_cast<std::underlying_type<std::decay<decltype(margin_mode)>::type>::type>(margin_mode);
  }
};

}  // namespace cache
}  // namespace roq
