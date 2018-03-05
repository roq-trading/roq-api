/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/generator.h"

using namespace quinclas::common;  // NOLINT

namespace examples {
namespace reference {

const size_t MAX_COLUMNS = 40;

const char *TIME_FORMAT_FILE = "%Y%m%d %H:%M:%S";

Generator::Generator(const std::string& path)
    : _csv_reader(path, MAX_COLUMNS) {
}

std::pair<bool, std::chrono::system_clock::time_point> Generator::fetch() {
  if (!_csv_reader.fetch())
    return std::make_pair(false, std::chrono::system_clock::time_point());
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
    .message_id = 0,
    .exchange_time = std::chrono::time_point_cast<duration_t>(exchange_time),
    .receive_time = std::chrono::time_point_cast<duration_t>(_receive_time),
  };
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
  LOG(INFO) << market_by_price;
  MarketByPriceEvent event = { message_info, market_by_price };
  strategy.on(event);
}

}  // namespace reference
}  // namespace examples
