/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"
#include "roq/string_buffer.h"

namespace roq {

//! Fields required to cancel an existing order
struct ROQ_PUBLIC CancelOrder final {
  std::string_view account;              //!< Account name
  uint32_t order_id = {};                //!< Order identifier
  std::string_view routing_id;           //!< Routing identifier
  uint32_t version = {};                 //!< Version number (strictly increasing, optional)
  uint32_t conditional_on_version = {};  //!< Auto-reject if this version has positively failed (optional)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::CancelOrder> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::CancelOrder &value, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(account="{}", )"
        R"(order_id={}, )"
        R"(routing_id="{}", )"
        R"(version={}, )"
        R"(conditional_on_version={})"
        R"(}})"_sv,
        value.account,
        value.order_id,
        value.routing_id,
        value.version,
        value.conditional_on_version);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::CancelOrder> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Event<roq::CancelOrder> &event, Context &context) {
    using namespace roq::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(cancel_order={})"
        R"(}})"_sv,
        event.message_info,
        event.value);
  }
};
