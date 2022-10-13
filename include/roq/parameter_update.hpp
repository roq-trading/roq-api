/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include <span>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/parameter.hpp"
#include "roq/trace.hpp"
#include "roq/update_type.hpp"

namespace roq {

//! Update relating to parameters
struct ROQ_PUBLIC ParameterUpdate final {
  std::span<Parameter> parameters;  //!< List of parameters
  UpdateType update_type = {};      //!< Update type
};

template <>
inline constexpr std::string_view get_name<ParameterUpdate>() {
  using namespace std::literals;
  return "parameter_update"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::ParameterUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::ParameterUpdate const &value, Context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(parameters=[{}], )"
        R"(update_type={})"
        R"(}})"sv,
        fmt::join(value.parameters, ", "sv),
        value.update_type);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::ParameterUpdate>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Event<roq::ParameterUpdate> const &event, Context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(parameter_update={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::ParameterUpdate>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Trace<roq::ParameterUpdate> const &event, Context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(parameter_update={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
