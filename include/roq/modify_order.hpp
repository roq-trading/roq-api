/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/numbers.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Fields required to modify an existing order
struct ROQ_PUBLIC ModifyOrder final {
  std::string_view account;              //!< Account name
  uint32_t order_id = {};                //!< Order identifier
  double quantity = NaN;                 //!< New (total) quantity
  double price = NaN;                    //!< New limit price
  std::string_view routing_id;           //!< Routing identifier
  uint32_t version = {};                 //!< Version number (strictly increasing, optional)
  uint32_t conditional_on_version = {};  //!< Auto-reject if this version has positively failed (optional)
};

template <>
inline constexpr std::string_view get_name<ModifyOrder>() {
  using namespace std::literals;
  return "modify_order"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::ModifyOrder> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::ModifyOrder const &value, Context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(account="{}", )"
        R"(order_id={}, )"
        R"(quantity={}, )"
        R"(price={}, )"
        R"(routing_id="{}", )"
        R"(version={}, )"
        R"(conditional_on_version={})"
        R"(}})"sv,
        value.account,
        value.order_id,
        value.quantity,
        value.price,
        value.routing_id,
        value.version,
        value.conditional_on_version);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::ModifyOrder> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Event<roq::ModifyOrder> const &event, Context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(modify_order={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::ModifyOrder> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Trace<roq::ModifyOrder> const &event, Context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(modify_order={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
