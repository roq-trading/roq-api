/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "collector/collector.h"
#include <glog/logging.h>

#include <limits>

using namespace quinclas::common;  // NOLINT

namespace examples {
namespace collector {

const char *DELIMITER = ",";
const char *TIME_FORMAT = "%E4Y-%m-%dT%H:%M:%E6S";
const auto TIME_ZONE = cctz::utc_time_zone();

Collector::Collector(Strategy::Dispatcher& dispatcher) : _dispatcher(dispatcher) {}

void Collector::on(const MarketByPriceEvent& event) {
  // convenience
  const auto& message_info = event.message_info;
  const auto& market_by_price = event.market_by_price;
  const auto& depth = market_by_price.depth;
  // current time
  const auto now = std::chrono::time_point_cast<duration_t>(std::chrono::system_clock::now());
  // log to stdout
  std::cout <<
    cctz::format(TIME_FORMAT, message_info.exchange_time, TIME_ZONE) << DELIMITER <<
    cctz::format(TIME_FORMAT, message_info.receive_time, TIME_ZONE) << DELIMITER <<
    cctz::format(TIME_FORMAT, now, TIME_ZONE) << DELIMITER <<
    market_by_price.exchange << DELIMITER <<
    market_by_price.instrument << DELIMITER <<
    depth[0].bid_price << DELIMITER <<
    depth[0].bid_quantity << DELIMITER <<
    depth[0].ask_price << DELIMITER <<
    depth[0].ask_quantity <<
    std::endl;
}

}  // namespace collector
}  // namespace examples
