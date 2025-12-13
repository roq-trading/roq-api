/* Copyright (c) 2017-2026, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include <magic_enum/magic_enum_format.hpp>

#include <string_view>

#include "roq/error.hpp"
#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/origin.hpp"
#include "roq/request_status.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Control ack
struct ROQ_PUBLIC ControlAck final {
  std::string_view request_id;             //!< Request identifier
  std::string_view user;                   //!< User name (client routing, optional)
  roq::Origin origin = {};                 //!< Origin of ack
  roq::RequestStatus request_status = {};  //!< Request status
  roq::Error error = {};                   //!< Error code
  std::string_view text;                   //!< Descriptive text
};

template <>
constexpr std::string_view get_name<ControlAck>() {
  using namespace std::literals;
  return "control_ack"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::ControlAck> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::ControlAck const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(request_id="{}", )"
        R"(user="{}", )"
        R"(origin={}, )"
        R"(request_status={}, )"
        R"(error={}, )"
        R"(text="{}")"
        R"(}})"sv,
        value.request_id,
        value.user,
        value.origin,
        value.request_status,
        value.error,
        value.text);
  }
};