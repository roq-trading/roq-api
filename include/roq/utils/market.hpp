/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/mbo_update.hpp"

namespace roq {
namespace utils {

inline constexpr bool clear_price_level(MBOUpdate const &update) {
  return !std::isnan(update.price) && std::empty(update.order_id) && update.action == UpdateAction::DELETE;
}

}  // namespace utils
}  // namespace roq
