/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/core.h>

#include "roq/limits.hpp"
#include "roq/side.hpp"
#include "roq/string_types.hpp"
#include "roq/update_action.hpp"
#include "roq/update_reason.hpp"

namespace roq {

//! Represents an update to be applied to an order book
struct ROQ_PUBLIC MBOUpdate final {
  double price = roq::NaN;        //!< Price level
  double quantity = {};           //!< Order quantity (remaining unless update action is fill)
  uint64_t priority = {};         //!< Queue priority (optional)
  roq::MBOOrderId order_id;       //!< Order identifier (optional when deleting)
  roq::Side side = {};            //!< Order side (optional when updating an existing order)
  roq::UpdateAction action = {};  //!< Update action
  roq::UpdateReason reason = {};  //!< Update reason (optional)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::MBOUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::MBOUpdate const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(price={}, )"
        R"(quantity={}, )"
        R"(priority={}, )"
        R"(order_id="{}", )"
        R"(side={}, )"
        R"(action={}, )"
        R"(reason={})"
        R"(}})"sv,
        value.price,
        value.quantity,
        value.priority,
        value.order_id,
        value.side,
        value.action,
        value.reason);
  }
};
