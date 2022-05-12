/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/string.hpp"

namespace roq {

namespace detail {
static const constexpr size_t MAX_LENGTH_SOURCE = 16;
static const constexpr size_t MAX_LENGTH_USER = 16;

static const constexpr size_t MAX_LENGTH_ACCOUNT = 32;
static const constexpr size_t MAX_LENGTH_EXCHANGE = 32;
static const constexpr size_t MAX_LENGTH_SYMBOL = 48;
static const constexpr size_t MAX_LENGTH_CURRENCY = 32;

static const constexpr size_t MAX_LENGTH_ORDER_TEMPLATE = 16;
static const constexpr size_t MAX_LENGTH_EXTERNAL_ACCOUNT = 64;
static const constexpr size_t MAX_LENGTH_EXTERNAL_ORDER_ID = 64;
static const constexpr size_t MAX_LENGTH_EXTERNAL_TRADE_ID = 40;
static const constexpr size_t MAX_LENGTH_ROUTING_ID = 16;

static const constexpr size_t MAX_LENGTH_CL_ORD_ID = 36;
static const constexpr size_t MAX_LENGTH_REQUEST_ID = 36;

static const constexpr size_t MAX_LENGTH_LABEL = 32;
static const constexpr size_t MAX_LENGTH_MEASUREMENT_KEY = 8;

static const constexpr size_t MAX_LENGTH_DESCRIPTION = 128;
static const constexpr size_t MAX_LENGTH_TIME_ZONE = 32;
}  // namespace detail

struct Source final : public String<detail::MAX_LENGTH_USER> {
  using String<detail::MAX_LENGTH_USER>::String;
};

struct User final : public String<detail::MAX_LENGTH_USER> {
  using String<detail::MAX_LENGTH_USER>::String;
};

struct Account final : public String<detail::MAX_LENGTH_ACCOUNT> {
  using String<detail::MAX_LENGTH_ACCOUNT>::String;
};

struct Exchange final : public String<detail::MAX_LENGTH_EXCHANGE> {
  using String<detail::MAX_LENGTH_EXCHANGE>::String;
};

struct Symbol final : public String<detail::MAX_LENGTH_SYMBOL> {
  using String<detail::MAX_LENGTH_SYMBOL>::String;
};

struct Currency final : public String<detail::MAX_LENGTH_CURRENCY> {
  using String<detail::MAX_LENGTH_CURRENCY>::String;
};

struct OrderTemplate final : public String<detail::MAX_LENGTH_ORDER_TEMPLATE> {
  using String<detail::MAX_LENGTH_ORDER_TEMPLATE>::String;
};

struct ExternalAccount final : public String<detail::MAX_LENGTH_EXTERNAL_ACCOUNT> {
  using String<detail::MAX_LENGTH_EXTERNAL_ACCOUNT>::String;
};

struct ExternalOrderId final : public String<detail::MAX_LENGTH_EXTERNAL_ORDER_ID> {
  using String<detail::MAX_LENGTH_EXTERNAL_ORDER_ID>::String;
};

struct ExternalTradeId final : public String<detail::MAX_LENGTH_EXTERNAL_TRADE_ID> {
  using String<detail::MAX_LENGTH_EXTERNAL_TRADE_ID>::String;
};

struct RoutingId final : public String<detail::MAX_LENGTH_ROUTING_ID> {
  using String<detail::MAX_LENGTH_ROUTING_ID>::String;
};

struct ClOrdId final : public String<detail::MAX_LENGTH_CL_ORD_ID> {
  using String<detail::MAX_LENGTH_CL_ORD_ID>::String;
};

struct RequestId final : public String<detail::MAX_LENGTH_REQUEST_ID> {
  using String<detail::MAX_LENGTH_REQUEST_ID>::String;
};

struct Label final : public String<detail::MAX_LENGTH_LABEL> {
  using String<detail::MAX_LENGTH_LABEL>::String;
};

struct MeasurementKey final : public String<detail::MAX_LENGTH_MEASUREMENT_KEY> {
  using String<detail::MAX_LENGTH_MEASUREMENT_KEY>::String;
};

struct Description final : public String<detail::MAX_LENGTH_DESCRIPTION> {
  using String<detail::MAX_LENGTH_DESCRIPTION>::String;
};

struct TimeZone final : public String<detail::MAX_LENGTH_TIME_ZONE> {
  using String<detail::MAX_LENGTH_TIME_ZONE>::String;
};

// validate

static_assert(sizeof(Source) == detail::MAX_LENGTH_USER);
static_assert(sizeof(User) == detail::MAX_LENGTH_USER);
static_assert(sizeof(Account) == detail::MAX_LENGTH_ACCOUNT);
static_assert(sizeof(Exchange) == detail::MAX_LENGTH_EXCHANGE);
static_assert(sizeof(Symbol) == detail::MAX_LENGTH_SYMBOL);
static_assert(sizeof(Currency) == detail::MAX_LENGTH_CURRENCY);
static_assert(sizeof(OrderTemplate) == detail::MAX_LENGTH_ORDER_TEMPLATE);
static_assert(sizeof(ExternalAccount) == detail::MAX_LENGTH_EXTERNAL_ACCOUNT);
static_assert(sizeof(ExternalOrderId) == detail::MAX_LENGTH_EXTERNAL_ORDER_ID);
static_assert(sizeof(ExternalTradeId) == detail::MAX_LENGTH_EXTERNAL_TRADE_ID);
static_assert(sizeof(RoutingId) == detail::MAX_LENGTH_ROUTING_ID);
static_assert(sizeof(ClOrdId) == detail::MAX_LENGTH_CL_ORD_ID);
static_assert(sizeof(RequestId) == detail::MAX_LENGTH_REQUEST_ID);
static_assert(sizeof(Label) == detail::MAX_LENGTH_LABEL);
static_assert(sizeof(MeasurementKey) == detail::MAX_LENGTH_MEASUREMENT_KEY);
static_assert(sizeof(Description) == detail::MAX_LENGTH_DESCRIPTION);
static_assert(sizeof(TimeZone) == detail::MAX_LENGTH_TIME_ZONE);

}  // namespace roq
