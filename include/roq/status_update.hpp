/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/core.h>

#include <magic_enum/magic_enum_format.hpp>

#include <string_view>

#include "roq/connection_status.hpp"
#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/state.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Service status update
struct ROQ_PUBLIC StatusUpdate final {
  roq::ConnectionStatus connection_status = {};  //!< Service connection status
  roq::State state = {};                         //!< Service state
  std::string_view user;                         //!< User name of a connected service (optional, empty means self)
};

template <>
inline constexpr std::string_view get_name<StatusUpdate>() {
  using namespace std::literals;
  return "status_update"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::StatusUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::StatusUpdate const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(connection_status={}, )"
        R"(state={}, )"
        R"(user="{}")"
        R"(}})"sv,
        value.connection_status,
        value.state,
        value.user);
  }
};
