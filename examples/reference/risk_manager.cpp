/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/risk_manager.h"
#include <glog/logging.h>

namespace examples {
namespace reference {

// constructor

RiskManager::RiskManager(const Config& config, const PositionManager& position_manager)
    : _config(config), _position_manager(position_manager) {}

}  // namespace reference
}  // namespace examples
