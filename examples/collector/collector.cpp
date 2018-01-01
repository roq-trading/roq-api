/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "collector/collector.h"
#include <glog/logging.h>

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

void Collector::on(const MarketByPriceEvent& event) {
  // convenience
  const auto& message_info = event.message_info;
  const auto& market_by_price = event.market_by_price;
  const auto& depth = market_by_price.depth;
  // current time
  const auto now = std::chrono::time_point_cast<duration_t>(std::chrono::system_clock::now());
  // log to stdout
  std::cout <<
    message_info.exchange_time << DELIMITER <<
    message_info.receive_time << DELIMITER <<
    now << DELIMITER <<
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
