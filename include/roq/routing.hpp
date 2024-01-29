/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/format.h>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/quality_of_service.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Routing
struct ROQ_PUBLIC Routing final {
  uint8_t source = {};                       //!< source
  bool is_last = false;                      //!< is last?
  QualityOfService quality_of_service = {};  //!< quality of service
};

template <>
inline constexpr std::string_view get_name<Routing>() {
  using namespace std::literals;
  return "routing"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::Routing> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Routing const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(source={}, )"
        R"(is_last={}, )"
        R"(quality_of_service={})"
        R"(}})"sv,
        value.source,
        value.is_last,
        value.quality_of_service);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::Routing>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::Routing> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(routing={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::Routing>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::Routing> const &event, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(routing={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
