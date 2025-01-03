/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cstdint>

namespace roq {

//! Enumeration of a statistics type
enum class StatisticsType : uint8_t {
  UNDEFINED = 0,
  OPEN_PRICE,
  SETTLEMENT_PRICE,
  CLOSE_PRICE,
  OPEN_INTEREST,
  PRE_OPEN_INTEREST,
  PRE_SETTLEMENT_PRICE,
  PRE_CLOSE_PRICE,
  HIGHEST_TRADED_PRICE,
  LOWEST_TRADED_PRICE,
  UPPER_LIMIT_PRICE,
  LOWER_LIMIT_PRICE,
  INDEX_VALUE,
  MARGIN_RATE,
  FUNDING_RATE,
  FUNDING_RATE_PREDICTION,
  TRADE_VOLUME,
};

}  // namespace roq
