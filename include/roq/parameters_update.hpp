/* Copyright (c) 2017-2023, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/compile.h>
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
  std::span<Parameter const> parameters;  //!< List of parameters
  UpdateType update_type = {};            //!< Update type
  std::string_view user;                  //!< User name (optional, only relevant for drop-copy)
};

template <>
inline constexpr std::string_view get_name<ParametersUpdate>() {
  using namespace std::literals;
  return "parameters_update"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::ParametersUpdate> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::ParametersUpdate const &value, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(parameters=[{}], )"
        R"(update_type={}, )"
        R"(user="{}")"
        R"(}})"_cf,
        fmt::join(value.parameters, ", "sv),
        value.update_type,
        value.user);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::ParametersUpdate>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Event<roq::ParametersUpdate> const &event, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(parameters_update={}, )"
        R"(message_info={})"
        R"(}})"_cf,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::ParametersUpdate>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Trace<roq::ParametersUpdate> const &event, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(parameters_update={}, )"
        R"(trace_info={})"
        R"(}})"_cf,
        event.value,
        event.trace_info);
  }
};
