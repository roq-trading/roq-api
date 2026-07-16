/* Copyright (c) 2017-2026, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <magic_enum/magic_enum_format.hpp>

#include <chrono>
#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Initial handshake ack
struct ROQ_PUBLIC HandshakeAck final {
  std::chrono::nanoseconds request_time_utc = {};   //!< Request time UTC
  std::chrono::nanoseconds response_time_utc = {};  //!< Response time UTC
  uint64_t session_id = {};                         //!< Session ID
  bool success = false;                             //!< Success?
  std::string_view failure_reason;                  //!< Reason for failure
  std::string_view package_name;                    //!< Package name
  std::string_view package_version;                 //!< Package version
  std::string_view package_build_number;            //!< Package build number
  std::string_view hostname;                        //!< Hostname (server)
  std::string_view username;                        //!< Username (server)
  uint64_t process_id = {};                         //!< Process id
  std::string_view service_name;                    //!< Service name
  std::string_view api_name;                        //!< API name
  uint8_t instance = {};                            //!< Instance id
  bool is_uat = false;                              //!< Is UAT?
};

template <>
constexpr std::string_view get_name<HandshakeAck>() {
  using namespace std::literals;
  return "handshake_ack"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::HandshakeAck> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::HandshakeAck const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(request_time_utc={}, )"
        R"(response_time_utc={}, )"
        R"(session_id={}, )"
        R"(success={}, )"
        R"(failure_reason="{}", )"
        R"(package_name="{}", )"
        R"(package_version="{}", )"
        R"(package_build_number="{}", )"
        R"(hostname="{}", )"
        R"(username="{}", )"
        R"(process_id={}, )"
        R"(service_name="{}", )"
        R"(api_name="{}", )"
        R"(instance={}, )"
        R"(is_uat={})"
        R"(}})"sv,
        value.request_time_utc,
        value.response_time_utc,
        value.session_id,
        value.success,
        value.failure_reason,
        value.package_name,
        value.package_version,
        value.package_build_number,
        value.hostname,
        value.username,
        value.process_id,
        value.service_name,
        value.api_name,
        value.instance,
        value.is_uat);
  }
};