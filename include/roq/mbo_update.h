/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/string_buffer.h"
#include "roq/uuid.h"

#include "roq/order_update_action.h"

namespace roq {

//! Represents the update status of a single order in the order book
struct ROQ_PUBLIC MBOUpdate final {
  double price = NaN;               //!< Order price
  double remaining_quantity = {};   //!< Remaining order quantity
  OrderUpdateAction action = {};    //!< Order update action
  uint32_t priority = {};           //!< Queue priority
  roq::string_buffer<40> order_id;  //!< Order identifier (as known to client)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::MBOUpdate> : public roq::formatter {
  template <typename Context>
  auto format(const roq::MBOUpdate &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(price={}, )"
        R"(remaining_quantity={}, )"
        R"(action={}, )"
        R"(priority={}, )"
        R"(order_id="{}")"
        R"(}})"_fmt,
        value.price,
        value.remaining_quantity,
        value.action,
        value.priority,
        value.order_id);
  }
};
