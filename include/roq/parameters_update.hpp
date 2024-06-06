/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include <span>
#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/parameter.hpp"
#include "roq/trace.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Update relating to parameters
struct ROQ_PUBLIC ParametersUpdate final {
  std::span<roq::Parameter const> parameters;  //!< List of parameters
  roq::UpdateType update_type = {};            //!< Update type
  std::string_view user;                       //!< User name (optional, only relevant for drop-copy)
};

template <>
inline constexpr std::string_view get_name<ParametersUpdate>() {
  using namespace std::literals;
  return "parameters_update"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::ParametersUpdate> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::ParametersUpdate const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(parameters=[{}], )"
        R"(update_type={}, )"
        R"(user="{}")"
        R"(}})"sv,
        fmt::join(value.parameters, ", "sv),
        value.update_type,
        value.user);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::ParametersUpdate>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::ParametersUpdate> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(parameters_update={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::ParametersUpdate>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::ParametersUpdate> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(parameters_update={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
