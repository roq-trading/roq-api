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

namespace roq {

//! Represents the update status of a single aggregate price level in the order book
struct ROQ_PUBLIC MBPUpdate final {
  double price = NaN;              //!< Price level
  double quantity = {};            //!< Total quantity available at price
  double implied_quantity = NaN;   //!< Total implied quantity at price (optional)
  uint32_t price_level = {};       //!< Level of price (optional, 1-based indexing)
  uint32_t number_of_orders = {};  //!< Number of orders at price (optional)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::MBPUpdate> : public roq::formatter {
  template <typename Context>
  auto format(const roq::MBPUpdate &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(price={}, )"
        R"(quantity={}, )"
        R"(implied_quantity={}, )"
        R"(price_level={}, )"
        R"(number_of_orders={})"
        R"(}})"_sv,
        value.price,
        value.quantity,
        value.implied_quantity,
        value.price_level,
        value.number_of_orders);
  }
};
