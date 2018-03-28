/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>
#include <limits>
#include "reference/config.h"

namespace examples {
namespace reference {

class TradingModel final {
 public:
  explicit TradingModel(const Config& config);
  void on(const roq::common::TimerEvent&);
  void on(const roq::common::MarketByPriceEvent&);
  void on(const roq::common::TradeSummaryEvent&);
  // TODO(thraneh): something more realistic
  double get_model_price() const { return _model_price; }
  double get_target_position() const { return _target_position; }
 private:
  const Config& _config;
  double _model_price = std::numeric_limits<double>::quiet_NaN();
  double _target_position = 0.0;
  std::chrono::system_clock::time_point _next_update;
};

}  // namespace reference
}  // namespace examples
