/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <limits>

#include "roq/decimals.hpp"
#include "roq/layer.hpp"
#include "roq/numbers.hpp"
#include "roq/order_status.hpp"
#include "roq/request_status.hpp"
#include "roq/side.hpp"
#include "roq/update_type.hpp"

namespace roq {
namespace utils {

//! Extract price for \ref roq::Layer given \ref roq::Side.
inline constexpr double price_from_side(const Layer &layer, Side side) {
  switch (side) {
    case Side::UNDEFINED:
      break;
    case Side::BUY:
      return layer.bid_price;
    case Side::SELL:
      return layer.ask_price;
  }
  return NaN;
}

//! Test if update type is snapshot-like
inline constexpr bool is_snapshot(UpdateType update_type) {
  switch (update_type) {
    case UpdateType::UNDEFINED:
      break;  // note! false (== 0) was previously interpreted as incremental
    case UpdateType::SNAPSHOT:
      return true;
    case UpdateType::INCREMENTAL:
      break;
    case UpdateType::STALE:
      return true;  // note! should normally reset
  }
  return false;
}

//! Check if order was received
inline constexpr bool was_order_received(OrderStatus order_status) {
  switch (order_status) {
    case OrderStatus::UNDEFINED:
      break;
    case OrderStatus::SENT:
      return false;
    case OrderStatus::ACCEPTED:
    case OrderStatus::SUSPENDED:
    case OrderStatus::WORKING:
    case OrderStatus::STOPPED:
    case OrderStatus::COMPLETED:
    case OrderStatus::EXPIRED:
    case OrderStatus::CANCELED:
    case OrderStatus::REJECTED:
      return true;
  }
  return false;  // throw?
}

//! Check if order has reached a final (completed) status
inline constexpr bool is_order_complete(OrderStatus order_status) {
  switch (order_status) {
    case OrderStatus::UNDEFINED:
      break;
    case OrderStatus::SENT:
    case OrderStatus::ACCEPTED:
    case OrderStatus::SUSPENDED:
    case OrderStatus::WORKING:
      return false;
    case OrderStatus::STOPPED:
    case OrderStatus::COMPLETED:
    case OrderStatus::EXPIRED:
    case OrderStatus::CANCELED:
    case OrderStatus::REJECTED:
      return true;
  }
  return true;  // throw?
}

//! Map order status to request status
inline constexpr RequestStatus to_request_status(OrderStatus order_status) {
  switch (order_status) {
    case OrderStatus::UNDEFINED:
      break;
    case OrderStatus::SENT:
      return RequestStatus::FORWARDED;
    case OrderStatus::ACCEPTED:
    case OrderStatus::SUSPENDED:
    case OrderStatus::WORKING:
    case OrderStatus::STOPPED:
    case OrderStatus::COMPLETED:
    case OrderStatus::EXPIRED:
    case OrderStatus::CANCELED:
      return RequestStatus::ACCEPTED;
    case OrderStatus::REJECTED:
      return RequestStatus::REJECTED;
  }
  return {};
}

//! Check if request has positively reached a final (completed) state
inline constexpr bool has_request_completed(RequestStatus request_status) {
  switch (request_status) {
    case RequestStatus::UNDEFINED:
    case RequestStatus::FORWARDED:
      break;
      // note! definitely completed
    case RequestStatus::ACCEPTED:
    case RequestStatus::REJECTED:
    case RequestStatus::FAILED:
      return true;
      // note! we don't know the real status for these
    case RequestStatus::DISCONNECTED:
    case RequestStatus::TIMEOUT:
      break;
    default:
      break;
  }
  return false;
}

//! Check if request has maybe reached a final state (including "unknown" states)
inline constexpr bool has_request_maybe_completed(RequestStatus request_status) {
  switch (request_status) {
    case RequestStatus::UNDEFINED:
    case RequestStatus::FORWARDED:
      break;
      // note! definitely completed
    case RequestStatus::ACCEPTED:
    case RequestStatus::REJECTED:
    case RequestStatus::FAILED:
      return true;
      // note! we don't know the real status for these
    case RequestStatus::DISCONNECTED:
    case RequestStatus::TIMEOUT:
      return true;
    default:
      break;
  }
  return false;
}

//! Check if request has positively failed
inline constexpr bool has_request_failed(RequestStatus request_status) {
  switch (request_status) {
    case RequestStatus::UNDEFINED:
    case RequestStatus::FORWARDED:
    case RequestStatus::ACCEPTED:
      break;
      // note! definitely failed
    case RequestStatus::REJECTED:
    case RequestStatus::FAILED:
      return true;
      // note! we don't know the real status for these
    case RequestStatus::DISCONNECTED:
    case RequestStatus::TIMEOUT:
      break;
    default:
      break;
  }
  return false;
}

//! Check if request has maybe failed (including "unknown" states)
inline constexpr bool has_request_maybe_failed(RequestStatus request_status) {
  switch (request_status) {
    case RequestStatus::UNDEFINED:
    case RequestStatus::FORWARDED:
    case RequestStatus::ACCEPTED:
      break;
      // note! definitely failed
    case RequestStatus::REJECTED:
    case RequestStatus::FAILED:
      return true;
      // note! we don't know the real status for these
    case RequestStatus::DISCONNECTED:
    case RequestStatus::TIMEOUT:
      return true;
    default:
      break;
  }
  return false;
}

//! Check if request has positively succeeded
inline constexpr bool has_request_succeeded(RequestStatus request_status) {
  switch (request_status) {
    case RequestStatus::UNDEFINED:
    case RequestStatus::FORWARDED:
      break;
      // note! definitely succeeded
    case RequestStatus::ACCEPTED:
      return true;
      // note! definitely did *not* succeed
    case RequestStatus::REJECTED:
    case RequestStatus::FAILED:
      break;
      // note! we don't know the real status for these
    case RequestStatus::DISCONNECTED:
    case RequestStatus::TIMEOUT:
      break;
    default:
      break;
  }
  return false;
}

//! Compare requests
inline constexpr int compare_requests(RequestStatus lhs, RequestStatus rhs) {
  if (lhs == rhs)
    return 0;
  const constexpr std::array<int, RequestStatus::count()> priority{
      0,
      1,
      4,
      5,
      2,
      3,
      6,
  };
  static_assert(std::size(priority) == RequestStatus::count());
  static_assert(priority[0] == 0);
  static_assert(priority[RequestStatus::count() - 1] == 6);
  auto lhs_priority = priority[static_cast<RequestStatus::type_t>(lhs)];
  auto rhs_priority = priority[static_cast<RequestStatus::type_t>(rhs)];
  return lhs_priority < rhs_priority ? -1 : 1;
}

//! Get the opposite \ref Side.
inline constexpr Side invert(Side side) {
  switch (side) {
    case Side::UNDEFINED:
    case Side::BUY:
      return Side::SELL;
    case Side::SELL:
      return Side::BUY;
  }
  return side;
}

//! Get notional sign given \ref roq::Side
inline constexpr int sign(Side side) {
  switch (side) {
    case Side::UNDEFINED:
      break;
    case Side::BUY:
      return 1;
    case Side::SELL:
      return -1;
  }
  return 0;
}

//! Number of decimal digits
inline constexpr int8_t decimal_digits(Decimals decimals) {
  switch (decimals) {
    case Decimals::UNDEFINED:
      break;
    case Decimals::_0:
      return 0;
    case Decimals::_1:
      return 1;
    case Decimals::_2:
      return 2;
    case Decimals::_3:
      return 3;
    case Decimals::_4:
      return 4;
    case Decimals::_5:
      return 5;
    case Decimals::_6:
      return 6;
    case Decimals::_7:
      return 7;
    case Decimals::_8:
      return 8;
    case Decimals::_9:
      return 9;
    case Decimals::_10:
      return 10;
    case Decimals::_11:
      return 11;
    case Decimals::_12:
      return 12;
    case Decimals::_13:
      return 13;
    case Decimals::_14:
      return 14;
    case Decimals::_15:
      return 15;
  }
  return -1;
}

inline constexpr Decimals to_decimals(int8_t decimal_digits) {
  if (decimal_digits < 0 || decimal_digits > 15)
    return {};
  return Decimals{static_cast<std::underlying_type<Decimals>::type>(decimal_digits + 1)};
}

inline constexpr bool has_more_precision(Decimals lhs, Decimals rhs) {
  return lhs != Decimals::UNDEFINED ? lhs > rhs : false;
}

}  // namespace utils
}  // namespace roq
