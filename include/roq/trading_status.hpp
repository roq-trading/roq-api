/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/format.h>

#include <magic_enum.hpp>

namespace roq {

//! Enumeration of the tradig status of a symbol
enum class TradingStatus : uint8_t {
  UNDEFINED = 0,
  START_OF_DAY,         //!< No matching, no order actions
  PRE_OPEN,             //!< No matching, all order actions
  PRE_OPEN_NO_CANCEL,   //!< No matching, only new orders
  PRE_OPEN_FREEZE,      //!< Matching, no order actions
  OPEN,                 //!< Matching, all order actions
  FAST_MARKET,          //!< Same as Open, some settings could be relaxed by the exchange
  HALT,                 //!< No matching, only order cancellation
  CLOSE_NOT_FINAL,      //!< Same as Close, state required to support mid-session PreOpen
  PRE_CLOSE,            //!< No matching, all order actions
  PRE_CLOSE_NO_CANCEL,  //!< No matching, only new orders
  PRE_CLOSE_FREEZE,     //!< Matching, no order actions
  CLOSE,                //!< No matching, no order actions, good-for-day orders automatically canceled
  POST_CLOSE,           //!< No matching, all order actions (only with next-trading-day validity)
  END_OF_DAY,           //!< No matching, no order actions
};

}  // namespace roq

template <>
struct fmt::formatter<roq::TradingStatus> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::TradingStatus const &value, Context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};
