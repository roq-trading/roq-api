/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include <magic_enum/magic_enum_format.hpp>

#include <string_view>

#include "roq/connection_status.hpp"
#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/state.hpp"
#include "roq/trace.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Service update
struct ROQ_PUBLIC ServiceUpdate final {
  std::string_view user;                         //!< Service name (optional)
  std::string_view description;                  //!< Service description
  roq::ConnectionStatus connection_status = {};  //!< Service connection status
  roq::State state = {};                         //!< Service state
  roq::UpdateType update_type = {};              //!< Update type
};

template <>
constexpr std::string_view get_name<ServiceUpdate>() {
  using namespace std::literals;
  return "service_update"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::ServiceUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::ServiceUpdate const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(user="{}", )"
        R"(description="{}", )"
        R"(connection_status={}, )"
        R"(state={}, )"
        R"(update_type={})"
        R"(}})"sv,
        value.user,
        value.description,
        value.connection_status,
        value.state,
        value.update_type);
  }
};
