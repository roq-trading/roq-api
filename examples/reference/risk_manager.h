/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include "reference/config.h"
#include "reference/position_manager.h"

namespace examples {
namespace reference {

class RiskManager final {
 public:
  RiskManager(const Config& config, const PositionManager& position_manager);

 private:
  const Config& _config;
  const PositionManager& _position_manager;
};

}  // namespace reference
}  // namespace examples
