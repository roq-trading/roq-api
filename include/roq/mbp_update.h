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

namespace roq {

//! Represents the update status of a single aggregate price level in the order book
struct ROQ_PUBLIC MBPUpdate final {
  double price = std::numeric_limits<double>::quiet_NaN();  //!< Price level
  double quantity = 0.0;                                    //!< Total quantity available at price
};

}  // namespace roq

template <>
struct fmt::formatter<roq::MBPUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::MBPUpdate &value, Context &context) {
    return format_to(
        context.out(),
        R"({{)"
        R"(price={}, )"
        R"(quantity={})"
        R"(}})",
        value.price,
        value.quantity);
  }
};
