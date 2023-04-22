/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <string>

#include "roq/api.hpp"

#include "roq/utils/common.hpp"
#include "roq/utils/update.hpp"

namespace roq {
namespace cache {

struct TopOfBook final {
  TopOfBook() = default;

  TopOfBook(TopOfBook const &) = delete;
  TopOfBook(TopOfBook &&) = default;

  void clear() {
    layer = {};
    exchange_time_utc = {};
    exchange_sequence = {};
    sending_time_utc = {};
  }

  [[nodiscard]] bool operator()(roq::TopOfBook const &top_of_book) {
    auto dirty = false;
    if (utils::is_snapshot(top_of_book.update_type)) {
      dirty |= utils::update(layer, top_of_book.layer);
    } else {
      dirty |= utils::update_if_not_empty(layer.bid_price, top_of_book.layer.bid_price);
      dirty |= utils::update_if_not_empty(layer.bid_quantity, top_of_book.layer.bid_quantity);
      dirty |= utils::update_if_not_empty(layer.ask_price, top_of_book.layer.ask_price);
      dirty |= utils::update_if_not_empty(layer.ask_quantity, top_of_book.layer.ask_quantity);
    }
    dirty |= utils::update(exchange_time_utc, top_of_book.exchange_time_utc);
    dirty |= utils::update(exchange_sequence, top_of_book.exchange_sequence);
    // note! sending time not used to check if updated
    utils::update(sending_time_utc, top_of_book.sending_time_utc);
    return dirty;
  }

  [[nodiscard]] bool operator()(Event<roq::TopOfBook> const &event) { return (*this)(event.value); }

  template <typename Context>
  [[nodiscard]] roq::TopOfBook convert(Context const &context, UpdateType update_type) const {
    return {
        .exchange = context.exchange,
        .symbol = context.symbol,
        .layer = layer,
        .update_type = update_type,
        .exchange_time_utc = exchange_time_utc,
        .exchange_sequence = exchange_sequence,
        .sending_time_utc = sending_time_utc,
    };
  }
  template <typename Context>
  [[nodiscard]] roq::TopOfBook convert(Context const &context) const {
    return convert(context, UpdateType::SNAPSHOT);
  }

  Layer layer = {};
  std::chrono::nanoseconds exchange_time_utc = {};
  int64_t exchange_sequence = {};
  std::chrono::nanoseconds sending_time_utc = {};
};

}  // namespace cache
}  // namespace roq
