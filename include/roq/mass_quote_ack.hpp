/* Copyright (c) 2017-2025, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include "roq/compat.hpp"

#include <fmt/core.h>

#include <magic_enum/magic_enum_format.hpp>

#include <string_view>

#include "roq/error.hpp"
#include "roq/event.hpp"
#include "roq/name.hpp"
#include "roq/origin.hpp"
#include "roq/request_status.hpp"
#include "roq/trace.hpp"

namespace roq {

//! Acknowledgement that a mass quote request has been seen by gateway/exchange
struct ROQ_PUBLIC MassQuoteAck final {
  uint16_t stream_id = {};                 //!< Stream identifier
  std::string_view account;                //!< Account name
  uint32_t quote_id = {};                  //!< Quote ID
  roq::Origin origin = {};                 //!< Origin of ack
  roq::RequestStatus request_status = {};  //!< Request status
  roq::Error error = {};                   //!< Error code
  std::string_view text;                   //!< Descriptive text
  std::string_view request_id;             //!< Request identifier
  std::string_view external_account;       //!< External account name
};

template <>
inline constexpr std::string_view get_name<MassQuoteAck>() {
  using namespace std::literals;
  return "mass_quote_ack"sv;
}

}  // namespace roq

template <>
struct fmt::formatter<roq::MassQuoteAck> {
  constexpr auto parse(format_parse_context &context) { return std::begin(context); }
  auto format(roq::MassQuoteAck const &value, format_context &context) const {
    using namespace std::literals;
    return fmt::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(account="{}", )"
        R"(quote_id={}, )"
        R"(origin={}, )"
        R"(request_status={}, )"
        R"(error={}, )"
        R"(text="{}", )"
        R"(request_id="{}", )"
        R"(external_account="{}")"
        R"(}})"sv,
        value.stream_id,
        value.account,
        value.quote_id,
        value.origin,
        value.request_status,
        value.error,
        value.text,
        value.request_id,
        value.external_account);
  }
};
