/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <cctz/time_zone.h>

#include <iostream>
#include <string>
#include <utility>

namespace examples {
namespace reference {

struct Config {
  cctz::time_zone time_zone;
  std::string instrument;
  std::string exchange;
  std::pair<double, double> model_params;
  std::chrono::seconds order_timeout;
};

}  // namespace reference
}  // namespace examples

std::ostream& operator<<(std::ostream& stream, const examples::reference::Config& config);
