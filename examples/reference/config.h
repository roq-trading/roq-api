/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <cctz/time_zone.h>

#include <iostream>
#include <string>
#include <utility>

namespace examples {
namespace reference {

struct Config {
  // global configuration
  cctz::time_zone time_zone;
  // risk management
  double risk_limit;
  // order management
  std::chrono::seconds order_timeout;
  // model configuration
  std::string exchange;
  std::string instrument;
  std::pair<double, double> model_params;
};

std::ostream& operator<<(std::ostream& stream, const examples::reference::Config& config);

}  // namespace reference
}  // namespace examples
