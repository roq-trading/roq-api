/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/fixed_string.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/span.h"

#include "roq/order_status.h"
#include "roq/position_effect.h"
#include "roq/side.h"

namespace roq {

//! Update relating to current status of an order
struct ROQ_PUBLIC OrderUpdate final {
  OrderUpdate() = default;
  OrderUpdate(OrderUpdate &&) = default;
  OrderUpdate(const OrderUpdate &) = delete;

  std::string_view account;                     //!< Account name (as known to the gateway)
  uint32_t order_id = 0;                        //!< Order identifier (as known to client)
  std::string_view exchange;                    //!< Exchange name
  std::string_view symbol;                      //!< Symbol
  OrderStatus status = OrderStatus::UNDEFINED;  //!< Order status
  Side side = Side::UNDEFINED;                  //!< Side
  double price = std::numeric_limits<double>::quiet_NaN();               //!< Price
  double remaining_quantity = std::numeric_limits<double>::quiet_NaN();  //!< Remaining quantity
  double traded_quantity = std::numeric_limits<double>::quiet_NaN();     //!< Traded quantity
  PositionEffect position_effect = PositionEffect::UNDEFINED;            //!< Position effect
  std::string_view order_template;                //!< Order template (as known to the gateway)
  std::chrono::nanoseconds create_time_utc = {};  //!< Created timestamp (UTC)
  std::chrono::nanoseconds update_time_utc = {};  //!< Updated timestamp (UTC)
  uint32_t gateway_order_id = 0;                  //!< Order identifier (as known to gateway)
  std::string_view external_account;  //!< External account name (as known to broker or exchange)
  std::string_view
      external_order_id;  //!< External order identifier (as known to broker or exchange)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::OrderUpdate> : public roq::formatter {
  template <typename Context>
  auto format(const roq::OrderUpdate &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(account="{}", )"
        R"(order_id={}, )"
        R"(exchange="{}", )"
        R"(symbol="{}", )"
        R"(status={}, )"
        R"(side={}, )"
        R"(price={}, )"
        R"(remaining_quantity={}, )"
        R"(traded_quantity={}, )"
        R"(position_effect={}, )"
        R"(order_template="{}", )"
        R"(create_time_utc={}, )"
        R"(update_time_utc={}, )"
        R"(gateway_order_id={}, )"
        R"(external_account="{}", )"
        R"(external_order_id="{}")"
        R"(}})"_fmt,
        value.account,
        value.order_id,
        value.exchange,
        value.symbol,
        value.status,
        value.side,
        value.price,
        value.remaining_quantity,
        value.traded_quantity,
        value.position_effect,
        value.order_template,
        value.create_time_utc,
        value.update_time_utc,
        value.gateway_order_id,
        value.external_account,
        value.external_order_id);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::OrderUpdate> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::OrderUpdate> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(order_update={})"
        R"(}})"_fmt,
        event.message_info,
        event.value);
  }
};
