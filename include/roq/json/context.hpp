/* Copyright (c) 2017-2023, Hans Erik Thrane */

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
