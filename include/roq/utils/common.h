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
inline bool is_order_complete(const OrderStatus status) {
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
inline bool has_request_completed(const RequestStatus status) {
  switch (status) {
    case RequestStatus::UNDEFINED:
    case RequestStatus::FORWARDED:
      break;
      // note! definitely completed
    case RequestStatus::ACCEPTED:
    case RequestStatus::REJECTED:
      return true;
      // note! we don't know the real status for these
    case RequestStatus::DISCONNECTED:
    case RequestStatus::TIMEOUT:
    case RequestStatus::FAILED:
      break;
    default:
      break;
  }
  return false;
}

//! Check if request has failed
inline bool has_request_failed(const RequestStatus status) {
  switch (status) {
    case RequestStatus::UNDEFINED:
    case RequestStatus::FORWARDED:
    case RequestStatus::ACCEPTED:
      break;
      // note! definitely failed
    case RequestStatus::REJECTED:
      return true;
      // note! we don't know the real status for these
    case RequestStatus::DISCONNECTED:
    case RequestStatus::TIMEOUT:
    case RequestStatus::FAILED:
      return true;
    default:
      break;
  }
  return false;
}

//! Check if request has succeeded
inline bool has_request_succeeded(const RequestStatus status) {
  switch (status) {
    case RequestStatus::UNDEFINED:
    case RequestStatus::FORWARDED:
      break;
      // note! definitely succeeded
    case RequestStatus::ACCEPTED:
      return true;
      // note! definitely did *not* succeed
    case RequestStatus::REJECTED:
      break;
      // note! we don't know the real status for these
    case RequestStatus::DISCONNECTED:
    case RequestStatus::TIMEOUT:
    case RequestStatus::FAILED:
      break;
    default:
      break;
  }
  return false;
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
