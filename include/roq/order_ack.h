/* Copyright (c) 2017-2021, Hans Erik Thrane */

/* !!! THIS FILE HAS BEEN AUTO-GENERATED !!! */

#pragma once

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <chrono>
#include <limits>
#include <string_view>

#include "roq/chrono.h"
#include "roq/compat.h"
#include "roq/fixed_string.h"
#include "roq/span.h"

#include "roq/event.h"
#include "roq/message_info.h"

#include "roq/error.h"
#include "roq/origin.h"
#include "roq/request_status.h"
#include "roq/request_type.h"

namespace roq {

//! Acknowledgement that a create/modify/cancel order request has been seen by gateway/exchange
struct ROQ_PUBLIC OrderAck final {
  OrderAck() = default;
  OrderAck(OrderAck &&) = default;
  OrderAck(const OrderAck &) = delete;

  std::string_view account;                         //!< Account name (as known to the gateway)
  uint32_t order_id = 0;                            //!< Order identifier (as known to client)
  RequestType type = RequestType::UNDEFINED;        //!< Request type
  Origin origin = Origin::UNDEFINED;                //!< Origin of ack
  RequestStatus status = RequestStatus::UNDEFINED;  //!< Request status
  Error error = Error::UNDEFINED;                   //!< Error code
  std::string_view text;                            //!< Descriptive text
  uint32_t gateway_order_id = 0;                    //!< Order identifier (as known to gateway)
  std::string_view external_account;  //!< External account name (as known to broker or exchange)
  std::string_view
      external_order_id;        //!< External order identifier (as known to broker or exchange)
  std::string_view request_id;  //!< Request identifier (as sent to broker or exchange)
};

}  // namespace roq

template <>
struct fmt::formatter<roq::OrderAck> {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::OrderAck &value, Context &context) {
    using namespace std::literals;  // NOLINT
    return format_to(
        context.out(),
        R"({{)"
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
        R"(}})"sv,
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
struct fmt::formatter<roq::Event<roq::OrderAck> > {
  template <typename Context>
  constexpr auto parse(Context &context) {
    return context.begin();
  }
  template <typename Context>
  auto format(const roq::Event<roq::OrderAck> &event, Context &context) {
    using namespace std::literals;  // NOLINT
    return format_to(
        context.out(),
        R"({{)"
        R"(message_info={}, )"
        R"(order_ack={})"
        R"(}})"sv,
        event.message_info,
        event.value);
  }
};
