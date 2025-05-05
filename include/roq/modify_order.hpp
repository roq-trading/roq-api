/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include <magic_enum/magic_enum_format.hpp>

#include <string_view>

#include "roq/event.hpp"
#include "roq/limits.hpp"
#include "roq/name.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Fields required to modify an existing order
struct ROQ_PUBLIC ModifyOrder final {
  std::string_view account;              //!< Account name
  uint64_t order_id = {};                //!< Order identifier
  std::string_view request_template;     //!< Request template (gateway configured)
  double quantity = roq::NaN;            //!< New (total) quantity
  double price = roq::NaN;               //!< New limit price
  std::string_view routing_id;           //!< Routing identifier
  uint32_t version = {};                 //!< Version number (strictly increasing, optional)
  uint32_t conditional_on_version = {};  //!< Auto-reject if this version has positively failed (optional)
};

template <>
constexpr std::string_view get_name<ModifyOrder>() {
  using namespace std::literals;
  return "modify_order"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::ModifyOrder> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::ModifyOrder const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(account="{}", )"
        R"(order_id={}, )"
        R"(request_template="{}", )"
        R"(quantity={}, )"
        R"(price={}, )"
        R"(routing_id="{}", )"
        R"(version={}, )"
        R"(conditional_on_version={})"
        R"(}})"sv,
        value.account,
        value.order_id,
        value.request_template,
        value.quantity,
        value.price,
        value.routing_id,
        value.version,
        value.conditional_on_version);
  }
};
