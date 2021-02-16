/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/fixed_string.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/uuid.h"

namespace roq {

//! Represents the update status of a single aggregate price level in the order book
struct ROQ_PUBLIC MBPUpdate final {
  double price = NaN;    //!< Price level
  double quantity = {};  //!< Total quantity available at price
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
        R"(quantity={})"
        R"(}})"_fmt,
        value.price,
        value.quantity);
  }
};
