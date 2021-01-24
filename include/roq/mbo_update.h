/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/fixed_string.h"
#include "roq/span.h"
#include "roq/uuid.h"

#include "roq/order_update_action.h"

namespace roq {

//! Represents the update status of a single order in the order book
struct ROQ_PUBLIC MBOUpdate final {
  double price = std::numeric_limits<double>::quiet_NaN();  //!< Order price
  double remaining_quantity = 0.0;                          //!< Remaining order quantity
  OrderUpdateAction action = OrderUpdateAction::UNDEFINED;  //!< Order update action
  uint32_t priority = 0;                                    //!< Queue priority
  roq::fixed_string<40> order_id;  //!< Order identifier (as known to client)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::MBOUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::MBOUpdate &value, Context &context) {
    return format_to(
        context.out(),

        R"({{)"
        R"(price={}, )"
        R"(remaining_quantity={}, )"
        R"(action={}, )"
        R"(priority={}, )"
        R"(order_id="{}")"

        R"(}})",
        value.price,
        value.remaining_quantity,
        value.action,
        value.priority,
        value.order_id);
  }
};
