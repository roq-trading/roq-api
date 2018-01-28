/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <cctz/time_zone.h>

#include <string>
#include <utility>

namespace examples {
namespace reference {

struct Config {
  cctz::time_zone time_zone;
  std::string instrument;
  std::string exchange;
  std::pair<double, double> model_params;
};

// TODO(thraneh): operator<<

}  // namespace reference
}  // namespace examples
