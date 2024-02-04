/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/precision.hpp"

namespace roq {
namespace json {

struct Context final {
  Precision price_decimals;
  Precision quantity_decimals;
};

}  // namespace json
}  // namespace roq
