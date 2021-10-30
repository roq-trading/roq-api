/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <chrono>

#include "roq/api.h"

#include "roq/utils/update.h"

namespace roq {
namespace cache {

class ReferenceData final {
 public:
  ReferenceData(const std::string_view &exchange, const std::string_view &symbol)
      : exchange(exchange), symbol(symbol) {}

  ReferenceData(const ReferenceData &) = delete;
  ReferenceData(ReferenceData &&) = default;

  void clear() {
    stream_id = {};
    description.clear();
    security_type = {};
    base_currency.clear();
    quote_currency.clear();
    commission_currency.clear();
    tick_size = NaN;
    multiplier = NaN;
    min_trade_vol = NaN;
    max_trade_vol = NaN;
    trade_vol_step_size = NaN;
    option_type = {};
    strike_currency.clear();
    strike_price = NaN;
    underlying.clear();
    time_zone.clear();
    issue_date = {};
    settlement_date = {};
    expiry_datetime = {};
    expiry_datetime_utc = {};
  }

  [[nodiscard]] bool operator()(const roq::ReferenceData &reference_data) {
    auto dirty = false;
    dirty |= utils::update(stream_id, reference_data.stream_id);
    dirty |= utils::update(description, reference_data.description);
    dirty |= utils::update(security_type, reference_data.security_type);
    dirty |= utils::update(base_currency, reference_data.base_currency);
    dirty |= utils::update(quote_currency, reference_data.quote_currency);
    dirty |= utils::update(commission_currency, reference_data.commission_currency);
    dirty |= utils::update(tick_size, reference_data.tick_size);
    dirty |= utils::update(multiplier, reference_data.multiplier);
    dirty |= utils::update(min_trade_vol, reference_data.min_trade_vol);
    dirty |= utils::update(max_trade_vol, reference_data.max_trade_vol);
    dirty |= utils::update(trade_vol_step_size, reference_data.trade_vol_step_size);
    dirty |= utils::update(option_type, reference_data.option_type);
    dirty |= utils::update(strike_currency, reference_data.strike_currency);
    dirty |= utils::update(strike_price, reference_data.strike_price);
    dirty |= utils::update(underlying, reference_data.underlying);
    dirty |= utils::update(time_zone, reference_data.time_zone);
    dirty |= utils::update(issue_date, reference_data.issue_date);
    dirty |= utils::update(settlement_date, reference_data.settlement_date);
    dirty |= utils::update(expiry_datetime, reference_data.expiry_datetime);
    dirty |= utils::update(expiry_datetime_utc, reference_data.expiry_datetime_utc);
    return dirty;
  }

  [[nodiscard]] operator roq::ReferenceData() const {
    return {
        .stream_id = stream_id,
        .exchange = exchange,
        .symbol = symbol,
        .description = description,
        .security_type = security_type,
        .base_currency = base_currency,
        .quote_currency = quote_currency,
        .commission_currency = commission_currency,
        .tick_size = tick_size,
        .multiplier = multiplier,
        .min_trade_vol = min_trade_vol,
        .max_trade_vol = max_trade_vol,
        .trade_vol_step_size = trade_vol_step_size,
        .option_type = option_type,
        .strike_currency = strike_currency,
        .strike_price = strike_price,
        .underlying = underlying,
        .time_zone = time_zone,
        .issue_date = issue_date,
        .settlement_date = settlement_date,
        .expiry_datetime = expiry_datetime,
        .expiry_datetime_utc = expiry_datetime_utc,
    };
  }

  uint16_t stream_id = {};
  const string_buffer<MAX_LENGTH_EXCHANGE> exchange;
  const string_buffer<MAX_LENGTH_SYMBOL> symbol;
  string_buffer<MAX_LENGTH_DESCRIPTION> description;
  SecurityType security_type = {};
  string_buffer<MAX_LENGTH_CURRENCY> base_currency;
  string_buffer<MAX_LENGTH_CURRENCY> quote_currency;
  string_buffer<MAX_LENGTH_CURRENCY> commission_currency;
  double tick_size = NaN;
  double multiplier = NaN;
  double min_trade_vol = NaN;
  double max_trade_vol = NaN;
  double trade_vol_step_size = NaN;
  OptionType option_type = {};
  string_buffer<MAX_LENGTH_CURRENCY> strike_currency;
  double strike_price = NaN;
  string_buffer<MAX_LENGTH_SYMBOL> underlying;
  string_buffer<MAX_LENGTH_TIME_ZONE> time_zone;
  roq::chrono::days issue_date = {};
  roq::chrono::days settlement_date = {};
  std::chrono::seconds expiry_datetime = {};
  std::chrono::seconds expiry_datetime_utc = {};
};

}  // namespace cache
}  // namespace roq
