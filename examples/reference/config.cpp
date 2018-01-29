/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/config.h"
#include <glog/logging.h>

namespace examples {
namespace reference {

}  // namespace reference
}  // namespace examples

std::ostream& operator<<(std::ostream& stream, const examples::reference::Config& config) {
  return stream << "Config = {"
      "time_zone=\"" << config.time_zone.name() << "\", "
      "instrument=\"" << config.instrument << "\", "
      "exchange=\"" << config.exchange << "\", "
      "order_timeout=" << config.order_timeout.count() << ", "
      "model_params=(" <<
      config.model_params.first << ", " <<
      config.model_params.second <<
      ")"
      "}";
}
