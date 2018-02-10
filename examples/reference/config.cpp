/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/config.h"
#include <quinclas/logging.h>
#include <iomanip>

namespace examples {
namespace reference {

std::ostream& operator<<(std::ostream& stream, const examples::reference::Config& config) {
  return stream << "Config = {"
      "time_zone=\"" << config.time_zone.name() << "\", "
      "risk_limit=" << config.risk_limit << ", "
      "order_timeout=" << config.order_timeout.count() << ", "
      "exchange=\"" << config.exchange << "\", "
      "instrument=\"" << config.instrument << "\", "
      "model_params=(" <<
      config.model_params.first << ", " <<
      config.model_params.second <<
      ")"
      "}";
}

}  // namespace reference
}  // namespace examples
