/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <limits>

#include "roq/layer.h"
#include "roq/numbers.h"
#include "roq/order_status.h"
#include "roq/request_status.h"
#include "roq/side.h"

namespace roq {
namespace utils {

//! Extract price for \ref roq::Layer given \ref roq::Side.
inline double price_from_side(const Layer &layer, Side side) {
  switch (side) {
    case Side::BUY:
      return layer.bid_price;
    case Side::SELL:
      return layer.ask_price;
    default:
      return NaN;
  }
}

//! Check if order has reached a final (completed) status
inline bool is_order_complete(const OrderStatus &status) {
  switch (status) {
    case OrderStatus::SENT:
    case OrderStatus::ACCEPTED:
    case OrderStatus::SUSPENDED:
    case OrderStatus::WORKING:
      return false;
    case OrderStatus::COMPLETED:
    case OrderStatus::EXPIRED:
    case OrderStatus::CANCELED:
    case OrderStatus::REJECTED:
      return true;
    default:
      // XXX throw?
      return true;
  }
}

//! Check if request has reached a final (completed) status
inline bool is_request_complete(const RequestStatus &status) {
  switch (status) {
    case RequestStatus::FORWARDED:
      return false;
    case RequestStatus::ACCEPTED:
    case RequestStatus::REJECTED:
    case RequestStatus::TIMEOUT:
      return true;
    default:
      // XXX throw?
      return true;
  }
}

//! Get the opposite \ref Side.
inline Side invert(Side side) {
  switch (side) {
    case Side::BUY:
      return Side::SELL;
    case Side::SELL:
      return Side::BUY;
    default:
      return side;
  }
}

//! Get notional sign given \ref roq::Side
inline int sign(Side side) {
  switch (side) {
    case Side::BUY:
      return 1;
    case Side::SELL:
      return -1;
    default:
      return 0;
  }
}

}  // namespace utils
}  // namespace roq
