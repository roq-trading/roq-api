/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/string.hpp"

namespace roq {

struct Source final : public String<MAX_LENGTH_USER> {
  using String<MAX_LENGTH_USER>::String;
};

struct User final : public String<MAX_LENGTH_USER> {
  using String<MAX_LENGTH_USER>::String;
};

struct Account final : public String<MAX_LENGTH_ACCOUNT> {
  using String<MAX_LENGTH_ACCOUNT>::String;
};

struct Exchange final : public String<MAX_LENGTH_EXCHANGE> {
  using String<MAX_LENGTH_EXCHANGE>::String;
};

struct Symbol final : public String<MAX_LENGTH_SYMBOL> {
  using String<MAX_LENGTH_SYMBOL>::String;
};

struct Currency final : public String<MAX_LENGTH_CURRENCY> {
  using String<MAX_LENGTH_CURRENCY>::String;
};

struct OrderTemplate final : public String<MAX_LENGTH_ORDER_TEMPLATE> {
  using String<MAX_LENGTH_ORDER_TEMPLATE>::String;
};

struct ExternalAccount final : public String<MAX_LENGTH_EXTERNAL_ACCOUNT> {
  using String<MAX_LENGTH_EXTERNAL_ACCOUNT>::String;
};

struct ExternalOrderId final : public String<MAX_LENGTH_EXTERNAL_ORDER_ID> {
  using String<MAX_LENGTH_EXTERNAL_ORDER_ID>::String;
};

struct ExternalTradeId final : public String<MAX_LENGTH_EXTERNAL_TRADE_ID> {
  using String<MAX_LENGTH_EXTERNAL_TRADE_ID>::String;
};

struct RoutingId final : public String<MAX_LENGTH_ROUTING_ID> {
  using String<MAX_LENGTH_ROUTING_ID>::String;
};

struct ClOrdId final : public String<MAX_LENGTH_CL_ORD_ID> {
  using String<MAX_LENGTH_CL_ORD_ID>::String;
};

struct RequestId final : public String<MAX_LENGTH_REQUEST_ID> {
  using String<MAX_LENGTH_REQUEST_ID>::String;
};

struct Label final : public String<MAX_LENGTH_LABEL> {
  using String<MAX_LENGTH_LABEL>::String;
};

struct MeasurementKey final : public String<MAX_LENGTH_MEASUREMENT_KEY> {
  using String<MAX_LENGTH_MEASUREMENT_KEY>::String;
};

struct Description final : public String<MAX_LENGTH_DESCRIPTION> {
  using String<MAX_LENGTH_DESCRIPTION>::String;
};

struct TimeZone final : public String<MAX_LENGTH_TIME_ZONE> {
  using String<MAX_LENGTH_TIME_ZONE>::String;
};

}  // namespace roq
