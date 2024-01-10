/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <magic_enum.hpp>

#include <fmt/format.h>

namespace roq {

//! Last known order status received from exchange
enum class OrderStatus : uint8_t {
  UNDEFINED = 0,
  SENT,       //!< Order has been sent to exchange (no response has been received yet)
  ACCEPTED,   //!< Order has been accepted by exchange and is not yet been activated
  SUSPENDED,  //!< Order has been suspended and requires external action to re-activate
  WORKING,    //!< Order is working and has not yet been completely filled
  STOPPED,    //!< Order has guaranteed fill at as of yet unknown price
  COMPLETED,  //!< Order has been completely filled
  EXPIRED,    //!< Order has expired
  CANCELED,   //!< Order has been canceled
  REJECTED,   //!< Order has been rejected
};

}  // namespace roq

template <>
struct fmt::formatter<roq::OrderStatus> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::OrderStatus const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(context.out(), "{}"sv, magic_enum::enum_name(value));
  }
};
