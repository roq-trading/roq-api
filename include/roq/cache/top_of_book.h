/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <string>

#include "roq/api.h"

#include "roq/utils/update.h"

namespace roq {
namespace cache {

struct TopOfBook final {
  TopOfBook(const std::string_view &exchange, const std::string_view &symbol) : exchange(exchange), symbol(symbol) {}

  TopOfBook(const TopOfBook &) = delete;
  TopOfBook(TopOfBook &&) = default;

  void clear() {
    layer = {};
    exchange_time_utc = {};
  }

  [[nodiscard]] bool operator()(const roq::TopOfBook &top_of_book) {
    auto dirty = false;
    dirty |= utils::update(layer, top_of_book.layer);
    dirty |= utils::update(exchange_time_utc, top_of_book.exchange_time_utc);
    return dirty;
  }

  [[nodiscard]] bool operator()(const Event<roq::TopOfBook> &event) { return (*this)(event.value); }

  [[nodiscard]] operator roq::TopOfBook() const {
    return {
        .exchange = exchange,
        .symbol = symbol,
        .layer = layer,
        .exchange_time_utc = exchange_time_utc,
    };
  }

  const string_buffer<MAX_LENGTH_EXCHANGE> exchange;
  const string_buffer<MAX_LENGTH_SYMBOL> symbol;
  Layer layer = {};
  std::chrono::nanoseconds exchange_time_utc = {};
};

}  // namespace cache
}  // namespace roq
