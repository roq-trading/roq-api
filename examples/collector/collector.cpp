/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "collector/collector.h"

#include <cctz/time_zone.h>
#include <quinclas/logging.h>

#include <iostream>
#include <limits>

using namespace quinclas::common;  // NOLINT

namespace examples {
namespace collector {

namespace {
const char *DELIMITER = ",";
const char *TIME_FORMAT = "%E4Y-%m-%dT%H:%M:%E6S";
const auto TIME_ZONE = cctz::utc_time_zone();
std::ostream& operator<<(std::ostream& stream, quinclas::common::time_point_t time_point) {
  return stream << cctz::format(TIME_FORMAT, time_point, TIME_ZONE);
}
}  // namespace

void Collector::on(const BatchEndEvent&) {
  for (auto iter : _dirty)
    std::cout << *iter << std::endl;
  _dirty.clear();
}

void Collector::on(const MarketByPriceEvent& event) {
  get(event.market_by_price.instrument).update(event);
}

void Collector::on(const TradeSummaryEvent& event) {
  get(event.trade_summary.instrument).update(event);
}

Collector::State& Collector::get(const std::string& instrument) {
  auto iter = _cache.find(instrument);
  if (iter == _cache.end())
    iter = _cache.insert({instrument, State(instrument)}).first;
  auto& result = (*iter).second;
  _dirty.insert(&result);
  return result;
}

void Collector::State::update(
    const quinclas::common::MarketByPriceEvent& event) {
  exchange_time = event.market_by_price.exchange_time;
  receive_time = event.message_info.client_receive_time;
  std::memcpy(
      &top_of_book,
      &event.market_by_price.depth[0],
      sizeof(top_of_book));
}

void Collector::State::update(
    const quinclas::common::TradeSummaryEvent& event) {
  exchange_time = event.trade_summary.exchange_time;
  receive_time = event.message_info.client_receive_time;
  price = event.trade_summary.price;
  volume = event.trade_summary.volume;
  turnover = event.trade_summary.turnover;
}

std::ostream& operator<<(std::ostream& stream, Collector::State& state) {
  return stream <<
    state.exchange_time << DELIMITER <<
    state.receive_time << DELIMITER <<
    state.instrument << DELIMITER <<
    state.top_of_book.bid_price << DELIMITER <<
    state.top_of_book.bid_quantity << DELIMITER <<
    state.top_of_book.ask_price << DELIMITER <<
    state.top_of_book.ask_quantity << DELIMITER <<
    state.price << DELIMITER <<
    state.volume << DELIMITER <<
    state.turnover;
}

}  // namespace collector
}  // namespace examples
