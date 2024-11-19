/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/core.h>

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
  uint16_t stream_id = {};                       //!< Stream identifier
  std::string_view account;                      //!< Account name
  roq::Mask<roq::SupportType> supports;          //!< Supported update types
  roq::Transport transport = {};                 //!< Transport type (layer 4)
  roq::Protocol protocol = {};                   //!< Communication protocol (layer 7)
  roq::Mask<roq::Encoding> encoding;             //!< Message encoding
  roq::Priority priority = {};                   //!< Priority
  roq::ConnectionStatus connection_status = {};  //!< Connection status (when applicable)
  std::string_view interface;                    //!< Local network interface
  std::string_view authority;                    //!< URL authority (network end-point)
  std::string_view path;                         //!< URL path
  std::string_view proxy;                        //!< Network proxy
};

template <>
inline constexpr std::string_view get_name<StreamStatus>() {
  using namespace std::literals;
  return "stream_status"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::StreamStatus> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::StreamStatus const &value, format_context &context) const {
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
        R"(connection_status={}, )"
        R"(interface="{}", )"
        R"(authority="{}", )"
        R"(path="{}", )"
        R"(proxy="{}")"
        R"(}})"sv,
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
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Event<roq::StreamStatus> const &event, format_context &context) const {
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
struct fmt::formatter<roq::Trace<roq::StreamStatus>> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Trace<roq::StreamStatus> const &event, format_context &context) const {
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
