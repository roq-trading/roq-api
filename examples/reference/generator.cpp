/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/generator.h"

#include <quinclas/logging.h>

#include <limits>

using namespace quinclas::common;  // NOLINT

namespace examples {
namespace reference {

const size_t MAX_COLUMNS = 40;

const char *TIME_FORMAT_FILE = "%Y%m%d %H:%M:%S";

Generator::Generator(const std::string& path)
    : _csv_reader(path, MAX_COLUMNS) {}

Generator::~Generator() {
  LOG(INFO) << "Processed " << _message_id << " message(s)";
}

std::pair<bool, std::chrono::system_clock::time_point> Generator::fetch() {
  if (!_csv_reader.fetch())
    return std::make_pair(false, std::chrono::system_clock::time_point());
  ++_message_id;
  auto receive_time = _csv_reader.get_time_point(2, TIME_FORMAT_FILE);
  LOG_IF(FATAL, receive_time < _receive_time) << "Incorrect sequencing";
  _receive_time = receive_time;
  return std::make_pair(true, _receive_time);
}

void Generator::dispatch(quinclas::common::Strategy& strategy) {
  auto symbol = _csv_reader.get_string(0);
  auto exchange_time = _csv_reader.get_time_point(1, TIME_FORMAT_FILE);
  auto type = _csv_reader.get_integer(_csv_reader.length() - 1);  // last column is indicator for L1/L2
  switch (type) {
    case 0: return;  // L1 (don't process, for now)
    case 1: break;
    default: LOG(FATAL) << "Invalid type=" << type;
  }
  MessageInfo message_info = {
    .gateway = "SIM",
    .message_id = _message_id,
    .exchange_time = std::chrono::time_point_cast<duration_t>(exchange_time),
    .receive_time = std::chrono::time_point_cast<duration_t>(_receive_time),
  };
  strategy.on(BatchBeginEvent{ .message_info = message_info, });
  MarketByPrice market_by_price = {
    .exchange = "SIM",
    .instrument = symbol.c_str(),
    .depth = {}
  };
  for (auto i = 0; i < 5; ++i) {
    auto offset = 3 + (i * 4);
    Layer& layer = market_by_price.depth[i];
    layer.bid_price = _csv_reader.get_number(offset + 0);
    layer.bid_quantity = _csv_reader.get_number(offset + 1);
    layer.ask_price = _csv_reader.get_number(offset + 2);
    layer.ask_quantity = _csv_reader.get_number(offset + 3);
  }
  VLOG(1) << market_by_price;
  MarketByPriceEvent market_by_price_event = { message_info, market_by_price };
  strategy.on(market_by_price_event);
  TradeSummary trade_summary = {
    .exchange = "SIM",
    .instrument = symbol.c_str(),
    .price = _csv_reader.get_number(24),
    .volume = _csv_reader.get_number(34),
    .turnover = _csv_reader.get_number(33),
    .direction = quinclas::common::TradeDirection::Undefined,
  };
  VLOG(1) << trade_summary;
  TradeSummaryEvent trade_summary_event = { message_info, trade_summary };
  strategy.on(trade_summary_event);
  strategy.on(BatchEndEvent{ .message_info = message_info, });
}

}  // namespace reference
}  // namespace examples
