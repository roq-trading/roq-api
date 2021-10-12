/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <utility>

#include "roq/api.h"

namespace roq {
namespace market {

//! Interface
class ROQ_PUBLIC MarketByPrice {
 public:
  virtual ~MarketByPrice() {}

  virtual std::string_view exchange() const = 0;
  virtual std::string_view symbol() const = 0;

  // increments (used to convert between integer and floating point representation)
  // these values could be tick_size and min_trade_vol from ReferenceData, but
  // not necessarily -- it depends on adjustments potentially required by GatewaySettings
  virtual double price_increment() const = 0;
  virtual double quantity_increment() const = 0;

  // precision required to show all significant decimal places (-1 means undefined)
  virtual int8_t price_decimal_digits() const = 0;
  virtual int8_t quantity_decimal_digits() const = 0;

  // convert price/quantity to internal representation
  // note! some events can change the internal representation, e.g. ReferenceData
  virtual int64_t price_to_internal(double price) const = 0;
  virtual uint64_t quantity_to_internal(double quantity) const = 0;

  // convert internal representation to price/quantity
  // note! some events can change the internal representation, e.g. ReferenceData
  virtual double internal_to_price(int64_t price) const = 0;
  virtual double internal_to_quantity(uint64_t quantity) const = 0;

  virtual bool empty() const = 0;
  virtual void clear() = 0;

  virtual bool is_bad() const = 0;

  template <typename T>
  void operator()(const T &value) {
    update_helper(value);
  }

  // normalized update, e.g. unsorted update or "incremental" full image
  template <typename F>
  void operator()(
      const MarketByPriceUpdate &market_by_price_update,
      const roq::span<MBPUpdate> &final_bids,
      const roq::span<MBPUpdate> &final_asks,
      F callback) {
    auto [final_bids_size, final_asks_size] = update_helper(market_by_price_update, final_bids, final_asks);
    if (final_bids_size == 0 && final_asks_size == 0) {
      callback(market_by_price_update);
    } else {
      auto bids = final_bids_size ? roq::span{final_bids.data(), final_bids_size} : market_by_price_update.bids;
      auto asks = final_asks_size ? roq::span{final_asks.data(), final_asks_size} : market_by_price_update.asks;
      const MarketByPriceUpdate final_market_by_price_update{
          .stream_id = market_by_price_update.stream_id,
          .exchange = market_by_price_update.exchange,
          .symbol = market_by_price_update.symbol,
          .bids = bids,
          .asks = asks,
          .update_type = market_by_price_update.update_type,
          .exchange_time_utc = market_by_price_update.exchange_time_utc,
      };
      callback(final_market_by_price_update);
    }
  }

  // copy-out
  virtual size_t extract(const roq::span<Layer> &, bool fill_zero = false) const = 0;
  virtual std::pair<size_t, size_t> extract(
      const roq::span<MBPUpdate> &bids, const roq::span<MBPUpdate> &asks) const = 0;

  // atomic update to the order book (quantity == 0 means remove)
  void operator()(Side side, double price, double quantity) { update_helper(side, price, quantity); }

  // note! the following methods should not be considered stable

  using price_level_t = std::pair<int64_t, uint64_t>;

  virtual roq::span<price_level_t const> bids() const = 0;
  virtual roq::span<price_level_t const> asks() const = 0;

 protected:
  virtual void update_helper(const GatewaySettings &) = 0;
  virtual void update_helper(const ReferenceData &) = 0;
  virtual void update_helper(const MarketByPriceUpdate &) = 0;

  virtual std::pair<size_t, size_t> update_helper(
      const MarketByPriceUpdate &, const roq::span<MBPUpdate> &bids, const roq::span<MBPUpdate> &asks) = 0;

  virtual void update_helper(Side, double price, double quantity) = 0;
};

}  // namespace market
}  // namespace roq
