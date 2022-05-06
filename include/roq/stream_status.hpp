/* Copyright (c) 2017-2022, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <string_view>

#include "roq/compat.hpp"
#include "roq/event.hpp"
#include "roq/mask.hpp"
#include "roq/message_info.hpp"
#include "roq/name.hpp"
#include "roq/numbers.hpp"
#include "roq/span.hpp"
#include "roq/string_types.hpp"
#include "roq/trace.hpp"

#include "roq/connection_status.hpp"
#include "roq/encoding.hpp"
#include "roq/priority.hpp"
#include "roq/protocol.hpp"
#include "roq/support_type.hpp"
#include "roq/transport.hpp"

namespace roq {

//! Update relating to current stream status
struct ROQ_PUBLIC StreamStatus final {
  uint16_t stream_id = {};                  //!< Stream identifier
  std::string_view account;                 //!< Account name
  Mask<SupportType> supports;               //!< Supported update types
  Transport transport = {};                 //!< Transport type (layer 4)
  Protocol protocol = {};                   //!< Communication protocol (layer 7)
  Mask<Encoding> encoding;                  //!< Message encoding
  Priority priority = {};                   //!< Priority
  ConnectionStatus connection_status = {};  //!< Connection status (when applicable)
};

template <>
inline constexpr std::string_view get_name<StreamStatus>() {
  using namespace std::literals;
  return "stream_status"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::StreamStatus> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::StreamStatus &value, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(account="{}", )"
        R"(supports={}, )"
        R"(transport={}, )"
        R"(protocol={}, )"
        R"(encoding={}, )"
        R"(priority={}, )"
        R"(connection_status={})"
        R"(}})"sv,
        value.stream_id,
        value.account,
        value.supports,
        value.transport,
        value.protocol,
        value.encoding,
        value.priority,
        value.connection_status);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::StreamStatus> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::Event<roq::StreamStatus> &event, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_status={}, )"
        R"(message_info={})"
        R"(}})"sv,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::StreamStatus const> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(const roq::Trace<roq::StreamStatus const> &event, Context &context) {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_status={}, )"
        R"(trace_info={})"
        R"(}})"sv,
        event.value,
        event.trace_info);
  }
};
