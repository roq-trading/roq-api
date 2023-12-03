/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/decimals.hpp"

namespace roq {
namespace json {

struct Context final {
  Decimals price_decimals;
  Decimals quantity_decimals;
};

}  // namespace json
}  // namespace roq
