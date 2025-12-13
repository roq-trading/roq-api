/* Copyright (c) 2017-2026, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <cstdint>

namespace roq {

//! Enumeration of a statistics type
enum class StatisticsType : uint8_t {
  UNDEFINED = 0,
  OPEN_PRICE,               //!< Open price
  SETTLEMENT_PRICE,         //!< Settlement price
  CLOSE_PRICE,              //!< Close price
  OPEN_INTEREST,            //!< Open interest
  PRE_OPEN_INTEREST,        //!< Pre-open interest
  PRE_SETTLEMENT_PRICE,     //!< Pre-settlement price
  PRE_CLOSE_PRICE,          //!< Pre-close price
  HIGHEST_TRADED_PRICE,     //!< Highest traded price
  LOWEST_TRADED_PRICE,      //!< Lowest traded price
  UPPER_LIMIT_PRICE,        //!< Upper limit price
  LOWER_LIMIT_PRICE,        //!< Lower limit price
  INDEX_VALUE,              //!< Index value
  MARGIN_RATE,              //!< Margin rate
  FUNDING_RATE,             //!< Funding rate
  FUNDING_RATE_PREDICTION,  //!< Funding rate prediction
  TRADE_VOLUME,             //!< Trade volume
};

}  // namespace roq
