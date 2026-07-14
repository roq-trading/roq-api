/* Copyright (c) 2017-2026, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include <magic_enum/magic_enum_format.hpp>

#include <chrono>
#include <span>
#include <string_view>

#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/order_cancel_policy.hpp"
#include "roq/order_management.hpp"
#include "roq/string_types.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Initial handshake
struct ROQ_PUBLIC Handshake final {
  std::chrono::nanoseconds request_time_utc = {};   //!< Request time UTC
  std::string_view application_name;                //!< Application name
  std::string_view application_version;             //!< Application version
  std::string_view application_build_number;        //!< Application build number
  std::string_view hostname;                        //!< Hostname (server)
  std::string_view username;                        //!< Username (server)
  uint64_t process_id = {};                         //!< Process id
  std::string_view service_name;                    //!< Service name
  std::string_view login;                           //!< Login
  std::string_view password;                        //!< Password
  std::span<roq::Account const> accounts;           //!< Accounts
  roq::OrderCancelPolicy order_cancel_policy = {};  //!< Order cancel policy
  roq::OrderManagement order_management = {};       //!< Order management
};

template <>
constexpr std::string_view get_name<Handshake>() {
  using namespace std::literals;
  return "handshake"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::Handshake> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::Handshake const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(request_time_utc={}, )"
        R"(application_name="{}", )"
        R"(application_version="{}", )"
        R"(application_build_number="{}", )"
        R"(hostname="{}", )"
        R"(username="{}", )"
        R"(process_id={}, )"
        R"(service_name="{}", )"
        R"(login="{}", )"
        R"(password="{}", )"
        R"(accounts=[{}], )"
        R"(order_cancel_policy={}, )"
        R"(order_management={})"
        R"(}})"sv,
        value.request_time_utc,
        value.application_name,
        value.application_version,
        value.application_build_number,
        value.hostname,
        value.username,
        value.process_id,
        value.service_name,
        value.login,
        value.password,
        fmt::join(value.accounts, ", "sv),
        value.order_cancel_policy,
        value.order_management);
  }
};