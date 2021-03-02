/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>

#include <chrono>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/event.h"
#include "roq/fixed_string.h"
#include "roq/format.h"
#include "roq/literals.h"
#include "roq/message_info.h"
#include "roq/numbers.h"
#include "roq/span.h"

namespace roq {

//! Update relating to external latency
struct ROQ_PUBLIC ExternalLatency final {
  uint16_t stream_id = {};                //!< Stream identifier
  std::string_view name;                  //!< connection name
  std::chrono::nanoseconds latency = {};  //!< latency measurement (1-way)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::ExternalLatency> : public roq::formatter {
  template <typename Context>
  auto format(const roq::ExternalLatency &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(name="{}", )"
        R"(latency={})"
        R"(}})"_fmt,
        value.stream_id,
        value.name,
        value.latency);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::ExternalLatency> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::ExternalLatency> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(external_latency={})"
        R"(}})"_fmt,
        event.message_info,
        event.value);
  }
};
