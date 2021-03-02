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

#include "roq/error.h"
#include "roq/origin.h"
#include "roq/request_status.h"
#include "roq/request_type.h"

namespace roq {

//! Acknowledgement that a create/modify/cancel order request has been seen by gateway/exchange
struct ROQ_PUBLIC OrderAck final {
  uint16_t stream_id = {};                          //!< Stream identifier
  std::string_view account;                         //!< Account name (as known to the gateway)
  uint32_t order_id = {};                           //!< Order identifier (as known to client)
  RequestType type = RequestType::UNDEFINED;        //!< Request type
  Origin origin = Origin::UNDEFINED;                //!< Origin of ack
  RequestStatus status = RequestStatus::UNDEFINED;  //!< Request status
  Error error = Error::UNDEFINED;                   //!< Error code
  std::string_view text;                            //!< Descriptive text
  uint32_t gateway_order_id = {};                   //!< Order identifier (as known to gateway)
  std::string_view external_account;  //!< External account name (as known to broker or exchange)
  std::string_view
      external_order_id;        //!< External order identifier (as known to broker or exchange)
  std::string_view request_id;  //!< Request identifier (as sent to broker or exchange)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::OrderAck> : public roq::formatter {
  template <typename Context>
  auto format(const roq::OrderAck &value, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(stream_id={}, )"
        R"(account="{}", )"
        R"(order_id={}, )"
        R"(type={}, )"
        R"(origin={}, )"
        R"(status={}, )"
        R"(error={}, )"
        R"(text="{}", )"
        R"(gateway_order_id={}, )"
        R"(external_account="{}", )"
        R"(external_order_id="{}", )"
        R"(request_id="{}")"
        R"(}})"_fmt,
        value.stream_id,
        value.account,
        value.order_id,
        value.type,
        value.origin,
        value.status,
        value.error,
        value.text,
        value.gateway_order_id,
        value.external_account,
        value.external_order_id,
        value.request_id);
  }
};
template <>
struct fmt::formatter<roq::Event<roq::OrderAck> > : public roq::formatter {
  template <typename Context>
  auto format(const roq::Event<roq::OrderAck> &event, Context &context) {
    using namespace roq::literals;
    return roq::format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(order_ack={})"
        R"(}})"_fmt,
        event.message_info,
        event.value);
  }
};
