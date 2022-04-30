/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/numbers.hpp"
#include "roq/span.hpp"
#include "roq/string_types.hpp"
#include "roq/uuid.hpp"

#include "roq/order_update_action.hpp"

namespace roq {

//! Represents the update status of a single order in the order book
struct ROQ_PUBLIC MBOUpdate final {
  double price = NaN;              //!< Order price
  double remaining_quantity = {};  //!< Remaining order quantity
  OrderUpdateAction action = {};   //!< Order update action
  uint32_t priority = {};          //!< Queue priority
  ExternalTradeId order_id;        //!< Order identifier
};

}  // namespace roq

template <>
struct fmt::formatter<roq::MBOUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::MBOUpdate &value, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(price={}, )"
        R"(remaining_quantity={}, )"
        R"(action={}, )"
        R"(priority={}, )"
        R"(order_id="{}")"
        R"(}})"sv,
        value.price,
        value.remaining_quantity,
        value.action,
        value.priority,
        value.order_id);
  }
};
