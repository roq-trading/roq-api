/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

#include "roq/utils/update.hpp"

namespace roq {
namespace cache {

//! Trade (state)
struct ROQ_PUBLIC Trade final {
  Trade(uint32_t order_id) : order_id{order_id} {}

  Trade(Trade const &) = delete;
  Trade(Trade &&) = default;

  void clear() {}

  [[nodiscard]] bool operator()(TradeUpdate const &trade_update) {
    auto dirty = false;
    dirty |= utils::update(stream_id, trade_update.stream_id);
    dirty |= utils::update(side, trade_update.side);
    dirty |= utils::update(position_effect, trade_update.position_effect);
    dirty |= utils::update(create_time_utc, trade_update.create_time_utc);
    dirty |= utils::update(update_time_utc, trade_update.update_time_utc);
    dirty |= utils::update(external_account, trade_update.external_account);
    dirty |= utils::update(external_order_id, trade_update.external_order_id);
    if (std::size(fills) != std::size(trade_update.fills)) {
      fills.assign(std::begin(trade_update.fills), std::end(trade_update.fills));
      dirty |= true;
    } else {
      for (size_t i = 0; i < std::size(fills); ++i) {
        auto &lhs = fills[i];
        auto &rhs = trade_update.fills[i];
        dirty |= utils::update(lhs.external_trade_id, rhs.external_trade_id);
        dirty |= utils::update(lhs.quantity, rhs.quantity);
        dirty |= utils::update(lhs.price, rhs.price);
        dirty |= utils::update(lhs.liquidity, rhs.liquidity);
      }
    }
    // fills...
    dirty |= utils::update(routing_id, trade_update.routing_id);
    // some update types will always be published
    switch (trade_update.update_type) {
      using enum UpdateType;
      case UNDEFINED:
        break;
      case SNAPSHOT:
        dirty = true;
        break;
      case INCREMENTAL:
        break;
      case STALE:
        dirty = true;
        break;
    }
    return dirty;
  }

  template <typename Context>
  [[nodiscard]] TradeUpdate convert(Context const &context) {
    return {
        .stream_id = stream_id,
        .account = context.account,
        .order_id = order_id,
        .exchange = context.exchange,
        .symbol = context.symbol,
        .side = side,
        .position_effect = position_effect,
        .create_time_utc = create_time_utc,
        .update_time_utc = update_time_utc,
        .external_account = external_account,
        .external_order_id = external_order_id,
        .fills = fills,  // note! reason why the method is not const
        .routing_id = routing_id,
        .update_type = UpdateType::SNAPSHOT,  // note!
    };
  }

  uint16_t stream_id = {};

  uint32_t const order_id = {};
  Side side = {};
  PositionEffect position_effect = {};
  std::chrono::nanoseconds create_time_utc = {};
  std::chrono::nanoseconds update_time_utc = {};
  ExternalAccount external_account;
  ExternalOrderId external_order_id;
  std::vector<Fill> fills;
  RoutingId routing_id;
};

}  // namespace cache
}  // namespace roq
