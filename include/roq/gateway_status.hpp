/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include <magic_enum/magic_enum_format.hpp>

#include <string_view>

#include "roq/event.hpp"
#include "roq/mask.hpp"
#include "roq/name.hpp"
#include "roq/support_type.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Update relating to current gateway service status
struct ROQ_PUBLIC GatewayStatus final {
  std::string_view account;                 //!< Account name
  roq::Mask<roq::SupportType> supported;    //!< Supported update types
  roq::Mask<roq::SupportType> available;    //!< Available update types (union of all streams, one or more available)
  roq::Mask<roq::SupportType> unavailable;  //!< Unavailable update types (union of all streams, one or more unavailable)
};

template <>
constexpr std::string_view get_name<GatewayStatus>() {
  using namespace std::literals;
  return "gateway_status"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::GatewayStatus> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::GatewayStatus const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(account="{}", )"
        R"(supported={}, )"
        R"(available={}, )"
        R"(unavailable={})"
        R"(}})"sv,
        value.account,
        value.supported,
        value.available,
        value.unavailable);
  }
};