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
inline constexpr double price_from_side(Layer const &layer, Side side) {
  switch (side) {
    using enum Side;
    case UNDEFINED:
      break;
    case BUY:
      return layer.bid_price;
    case SELL:
      return layer.ask_price;
  }
  return NaN;
}

//! Test if update type is snapshot-like
inline constexpr bool is_snapshot(UpdateType update_type) {
  switch (update_type) {
    using enum UpdateType;
    case UNDEFINED:
      break;  // note! false (== 0) was previously interpreted as incremental
    case SNAPSHOT:
      return true;
    case INCREMENTAL:
      break;
    case STALE:
      return true;  // note! should normally reset
  }
  return false;
}

//! Check if order was received
inline constexpr bool was_order_received(OrderStatus order_status) {
  switch (order_status) {
    using enum OrderStatus;
    case UNDEFINED:
      break;
    case SENT:
      return false;
    case ACCEPTED:
    case SUSPENDED:
    case WORKING:
    case STOPPED:
    case COMPLETED:
    case EXPIRED:
    case CANCELED:
    case REJECTED:
      return true;
  }
  return false;  // throw?
}

//! Check if order has reached a final (completed) status
inline constexpr bool is_order_complete(OrderStatus order_status) {
  switch (order_status) {
    using enum OrderStatus;
    case UNDEFINED:
      break;
    case SENT:
    case ACCEPTED:
    case SUSPENDED:
    case WORKING:
      return false;
    case STOPPED:
    case COMPLETED:
    case EXPIRED:
    case CANCELED:
    case REJECTED:
      return true;
  }
  return true;  // throw?
}

//! Map order status to request status
inline constexpr RequestStatus to_request_status(OrderStatus order_status) {
  switch (order_status) {
    using enum OrderStatus;
    case UNDEFINED:
      break;
    case SENT:
      return RequestStatus::FORWARDED;
    case ACCEPTED:
    case SUSPENDED:
    case WORKING:
    case STOPPED:
    case COMPLETED:
    case EXPIRED:
    case CANCELED:
      return RequestStatus::ACCEPTED;
    case REJECTED:
      return RequestStatus::REJECTED;
  }
  return {};
}

//! Check if request has positively reached a final (completed) state
inline constexpr bool has_request_completed(RequestStatus request_status) {
  using enum RequestStatus;
  switch (request_status) {
    case UNDEFINED:
    case FORWARDED:
      break;
      // note! definitely completed
    case ACCEPTED:
    case REJECTED:
    case FAILED:
      return true;
      // note! we don't know the real status for these
    case DISCONNECTED:
    case TIMEOUT:
      break;
    default:
      break;
  }
  return false;
}

//! Check if request has maybe reached a final state (including "unknown" states)
inline constexpr bool has_request_maybe_completed(RequestStatus request_status) {
  using enum RequestStatus;
  switch (request_status) {
    case UNDEFINED:
    case FORWARDED:
      break;
      // note! definitely completed
    case ACCEPTED:
    case REJECTED:
    case FAILED:
      return true;
      // note! we don't know the real status for these
    case DISCONNECTED:
    case TIMEOUT:
      return true;
    default:
      break;
  }
  return false;
}

//! Check if request has positively failed
inline constexpr bool has_request_failed(RequestStatus request_status) {
  switch (request_status) {
    using enum RequestStatus;
    case UNDEFINED:
    case FORWARDED:
    case ACCEPTED:
      break;
      // note! definitely failed
    case REJECTED:
    case FAILED:
      return true;
      // note! we don't know the real status for these
    case DISCONNECTED:
    case TIMEOUT:
      break;
    default:
      break;
  }
  return false;
}

//! Check if request has maybe failed (including "unknown" states)
inline constexpr bool has_request_maybe_failed(RequestStatus request_status) {
  switch (request_status) {
    using enum RequestStatus;
    case UNDEFINED:
    case FORWARDED:
    case ACCEPTED:
      break;
      // note! definitely failed
    case REJECTED:
    case FAILED:
      return true;
      // note! we don't know the real status for these
    case DISCONNECTED:
    case TIMEOUT:
      return true;
    default:
      break;
  }
  return false;
}

//! Check if request has positively succeeded
inline constexpr bool has_request_succeeded(RequestStatus request_status) {
  switch (request_status) {
    using enum RequestStatus;
    case UNDEFINED:
    case FORWARDED:
      break;
      // note! definitely succeeded
    case ACCEPTED:
      return true;
      // note! definitely did *not* succeed
    case REJECTED:
    case FAILED:
      break;
      // note! we don't know the real status for these
    case DISCONNECTED:
    case TIMEOUT:
      break;
    default:
      break;
  }
  return false;
}

//! Compare requests
inline constexpr std::strong_ordering compare_requests(RequestStatus lhs, RequestStatus rhs) {
  if (lhs == rhs)
    return std::strong_ordering::equal;
  const constexpr std::array<int, magic_enum::enum_count<RequestStatus>()> priority{
      0,
      1,
      4,
      5,
      2,
      3,
      6,
  };
  static_assert(std::size(priority) == magic_enum::enum_count<RequestStatus>());
  static_assert(priority[0] == 0);
  static_assert(priority[magic_enum::enum_count<RequestStatus>() - 1] == 6);
  auto lhs_priority = priority[static_cast<std::underlying_type<decltype(lhs)>::type>(lhs)];
  auto rhs_priority = priority[static_cast<std::underlying_type<decltype(rhs)>::type>(rhs)];
  if (lhs_priority < rhs_priority)
    return std::strong_ordering::less;
  return std::strong_ordering::greater;
}

//! Get the opposite \ref Side.
inline constexpr Side invert(Side side) {
  switch (side) {
    using enum Side;
    case UNDEFINED:
    case BUY:
      return SELL;
    case SELL:
      return BUY;
  }
  return side;
}

//! Get notional sign given \ref roq::Side
inline constexpr int sign(Side side) {
  switch (side) {
    using enum Side;
    case UNDEFINED:
      break;
    case BUY:
      return 1;
    case SELL:
      return -1;
  }
  return 0;
}

//! Number of decimal digits
inline constexpr int8_t decimal_digits(Decimals decimals) {
  switch (decimals) {
    using enum Decimals;
    case UNDEFINED:
      break;
    case _0:
      return 0;
    case _1:
      return 1;
    case _2:
      return 2;
    case _3:
      return 3;
    case _4:
      return 4;
    case _5:
      return 5;
    case _6:
      return 6;
    case _7:
      return 7;
    case _8:
      return 8;
    case _9:
      return 9;
    case _10:
      return 10;
    case _11:
      return 11;
    case _12:
      return 12;
    case _13:
      return 13;
    case _14:
      return 14;
    case _15:
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
