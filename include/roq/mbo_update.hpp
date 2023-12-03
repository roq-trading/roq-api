/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/compile.h>
#include <fmt/format.h>

#include "roq/numbers.hpp"
#include "roq/side.hpp"
#include "roq/string_types.hpp"
#include "roq/update_action.hpp"
#include "roq/update_reason.hpp"

namespace roq {

//! Represents an update to be applied to an order book
struct ROQ_PUBLIC MBOUpdate final {
  double price = NaN;        //!< Price level
  double quantity = {};      //!< Order quantity (remaining unless update action is fill)
  uint64_t priority = {};    //!< Queue priority (optional)
  MBOOrderId order_id;       //!< Order identifier (optional when deleting)
  Side side = {};            //!< Order side (optional when updating an existing order)
  UpdateAction action = {};  //!< Update action
  UpdateReason reason = {};  //!< Update reason (optional)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::MBOUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::MBOUpdate const &value, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
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
        R"(}})"_cf,
        value.price,
        value.quantity,
        value.priority,
        value.order_id,
        value.side,
        value.action,
        value.reason);
  }
};
