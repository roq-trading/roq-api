/* Copyright (c) 2017-2024, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/compile.h>
#include <fmt/format.h>

#include <string_view>

#include "roq/connection_status.hpp"
#include "roq/encoding.hpp"
#include "roq/event.hpp"
#include "roq/mask.hpp"
#include "roq/name.hpp"
#include "roq/priority.hpp"
#include "roq/protocol.hpp"
#include "roq/support_type.hpp"
#include "roq/trace.hpp"
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
  std::string_view interface;               //!< Local network interface
  std::string_view authority;               //!< URL authority (network end-point)
  std::string_view path;                    //!< URL path
  std::string_view proxy;                   //!< Network proxy
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
  auto format(roq::StreamStatus const &value, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
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
        R"(connection_status={}, )"
        R"(interface="{}", )"
        R"(authority="{}", )"
        R"(path="{}", )"
        R"(proxy="{}")"
        R"(}})"_cf,
        value.stream_id,
        value.account,
        value.supports,
        value.transport,
        value.protocol,
        value.encoding,
        value.priority,
        value.connection_status,
        value.interface,
        value.authority,
        value.path,
        value.proxy);
  }
};

template <>
struct fmt::formatter<roq::Event<roq::StreamStatus>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Event<roq::StreamStatus> const &event, Context &context) const {
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_status={}, )"
        R"(message_info={})"
        R"(}})"_cf,
        event.value,
        event.message_info);
  }
};

template <>
struct fmt::formatter<roq::Trace<roq::StreamStatus>> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return std::begin(context);
  }
  template <typename Context>
  auto format(roq::Trace<roq::StreamStatus> const &event, Context &context) const {
    using namespace std::literals;
    using namespace fmt::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_status={}, )"
        R"(trace_info={})"
        R"(}})"_cf,
        event.value,
        event.trace_info);
  }
};
