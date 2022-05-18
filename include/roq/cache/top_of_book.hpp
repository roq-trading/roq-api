/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <string>

#include "roq/api.hpp"

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
  }

  [[nodiscard]] bool operator()(roq::TopOfBook const &top_of_book) {
    auto dirty = false;
    dirty |= utils::update(layer, top_of_book.layer);
    dirty |= utils::update(exchange_time_utc, top_of_book.exchange_time_utc);
    return dirty;
  }

  [[nodiscard]] bool operator()(Event<roq::TopOfBook> const &event) { return (*this)(event.value); }

  template <typename Context>
  [[nodiscard]] roq::TopOfBook convert(Context const &context) const {
    return {
        .exchange = context.exchange,
        .symbol = context.symbol,
        .layer = layer,
        .exchange_time_utc = exchange_time_utc,
    };
  }

  Layer layer = {};
  std::chrono::nanoseconds exchange_time_utc = {};
};

}  // namespace cache
}  // namespace roq
