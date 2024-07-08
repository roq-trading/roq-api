/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>
#include <fmt/ranges.h>

#include "roq/limits.hpp"
#include "roq/update_action.hpp"

namespace roq {

//! Represents the update status of a single aggregate price level in the order book
struct ROQ_PUBLIC MBPUpdate final {
  double price = roq::NaN;               //!< Price
  double quantity = {};                  //!< Total quantity available at price (zero means remove price level)
  double implied_quantity = roq::NaN;    //!< Total implied quantity at price (optional)
  uint16_t number_of_orders = {};        //!< Number of orders at price (optional)
  roq::UpdateAction update_action = {};  //!< Type of update action
  uint32_t price_level = {};             //!< Price level (0-based indexing)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::MBPUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::MBPUpdate const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(price={}, )"
        R"(quantity={}, )"
        R"(implied_quantity={}, )"
        R"(number_of_orders={}, )"
        R"(update_action={}, )"
        R"(price_level={})"
        R"(}})"sv,
        value.price,
        value.quantity,
        value.implied_quantity,
        value.number_of_orders,
        value.update_action,
        value.price_level);
  }
};
