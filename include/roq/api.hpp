/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/compat.hpp"

// convenience

#include "roq/numbers.hpp"

// enums

#include "roq/connection_status.hpp"
#include "roq/decimals.hpp"
#include "roq/error.hpp"
#include "roq/execution_instruction.hpp"
#include "roq/liquidity.hpp"
#include "roq/option_type.hpp"
#include "roq/order_cancel_policy.hpp"
#include "roq/order_management.hpp"
#include "roq/order_status.hpp"
#include "roq/order_type.hpp"
#include "roq/order_update_action.hpp"
#include "roq/origin.hpp"
#include "roq/position_effect.hpp"
#include "roq/priority.hpp"
#include "roq/rate_limit_type.hpp"
#include "roq/request_status.hpp"
#include "roq/request_type.hpp"
#include "roq/security_type.hpp"
#include "roq/side.hpp"
#include "roq/statistics_type.hpp"
#include "roq/stream_type.hpp"
#include "roq/support_type.hpp"
#include "roq/time_in_force.hpp"
#include "roq/trading_status.hpp"
#include "roq/update_type.hpp"

// helpers

#include "roq/fill.hpp"
#include "roq/layer.hpp"
#include "roq/mbo_update.hpp"
#include "roq/mbp_update.hpp"
#include "roq/measurement.hpp"
#include "roq/statistics.hpp"
#include "roq/trade.hpp"

// transport

#include "roq/message_info.hpp"

// control (autogen)

#include "roq/connected.hpp"
#include "roq/disconnected.hpp"
#include "roq/start.hpp"
#include "roq/stop.hpp"
#include "roq/timer.hpp"

// messages (autogen)

#include "roq/batch_begin.hpp"
#include "roq/batch_end.hpp"
#include "roq/cancel_all_orders.hpp"
#include "roq/cancel_order.hpp"
#include "roq/create_order.hpp"
#include "roq/custom_metrics.hpp"
#include "roq/custom_metrics_update.hpp"
#include "roq/download_begin.hpp"
#include "roq/download_end.hpp"
#include "roq/external_latency.hpp"
#include "roq/funds_update.hpp"
#include "roq/gateway_settings.hpp"
#include "roq/gateway_status.hpp"
#include "roq/market_by_order_update.hpp"
#include "roq/market_by_price_update.hpp"
#include "roq/market_status.hpp"
#include "roq/modify_order.hpp"
#include "roq/order_ack.hpp"
#include "roq/order_update.hpp"
#include "roq/position_update.hpp"
#include "roq/rate_limit_trigger.hpp"
#include "roq/reference_data.hpp"
#include "roq/statistics_update.hpp"
#include "roq/stream_status.hpp"
#include "roq/top_of_book.hpp"
#include "roq/trade_summary.hpp"
#include "roq/trade_update.hpp"

// misc

#include "roq/exceptions.hpp"

// version

#define ROQ_VERSION "0.8.3"

namespace roq {

// user id's (restricted)

static const constexpr uint8_t SOURCE_NONE = 0;
static const constexpr uint8_t SOURCE_SELF = 255;

// limits

static const constexpr size_t MAX_LENGTH_SOURCE = 16;
static const constexpr size_t MAX_LENGTH_USER = 16;

static const constexpr size_t MAX_LENGTH_ACCOUNT = 32;
static const constexpr size_t MAX_LENGTH_EXCHANGE = 32;
static const constexpr size_t MAX_LENGTH_SYMBOL = 32;
static const constexpr size_t MAX_LENGTH_CURRENCY = 32;

static const constexpr size_t MAX_LENGTH_ORDER_TEMPLATE = 16;
static const constexpr size_t MAX_LENGTH_EXTERNAL_ACCOUNT = 64;
static const constexpr size_t MAX_LENGTH_EXTERNAL_ORDER_ID = 64;
static const constexpr size_t MAX_LENGTH_EXTERNAL_TRADE_ID = 40;
static const constexpr size_t MAX_LENGTH_ROUTING_ID = 16;

static const constexpr size_t MAX_LENGTH_CL_ORD_ID = 36;
static const constexpr size_t MAX_LENGTH_REQUEST_ID = 36;

static const constexpr uint32_t MAX_ORDER_ID = (1 << 24) - 1;
static const constexpr uint32_t MAX_REQUEST_VERSION = (1 << 24) - 1;

static const constexpr size_t MAX_LENGTH_LABEL = 32;
static const constexpr size_t MAX_LENGTH_MEASUREMENT_KEY = 8;

static const constexpr size_t MAX_LENGTH_DESCRIPTION = 128;
static const constexpr size_t MAX_LENGTH_TIME_ZONE = 32;

// fixed-length strings

using Source = String<MAX_LENGTH_USER>;
using User = String<MAX_LENGTH_USER>;

using Account = String<MAX_LENGTH_ACCOUNT>;
using Exchange = String<MAX_LENGTH_EXCHANGE>;
using Symbol = String<MAX_LENGTH_SYMBOL>;
using Currency = String<MAX_LENGTH_CURRENCY>;

using OrderTemplate = String<MAX_LENGTH_ORDER_TEMPLATE>;
using ExternalAccount = String<MAX_LENGTH_EXTERNAL_ACCOUNT>;
using ExternalOrderId = String<MAX_LENGTH_EXTERNAL_ORDER_ID>;
using ExternalTradeId = String<MAX_LENGTH_EXTERNAL_TRADE_ID>;
using RoutingId = String<MAX_LENGTH_ROUTING_ID>;

using ClOrdId = String<MAX_LENGTH_CL_ORD_ID>;
using RequestId = String<MAX_LENGTH_REQUEST_ID>;

using Label = String<MAX_LENGTH_LABEL>;
using MeasurementKey = String<MAX_LENGTH_MEASUREMENT_KEY>;

using Description = String<MAX_LENGTH_DESCRIPTION>;
using TimeZone = String<MAX_LENGTH_TIME_ZONE>;

// validate auto-generated code

static_assert(sizeof(decltype(RateLimitTrigger::users)::value_type) == MAX_LENGTH_USER);
static_assert(sizeof(decltype(RateLimitTrigger::accounts)::value_type) == MAX_LENGTH_ACCOUNT);

static_assert(sizeof(decltype(Measurement::name)) == MAX_LENGTH_MEASUREMENT_KEY);

// check that multiples can be cache aligned

static_assert(sizeof(Layer) == 32);
static_assert(sizeof(MBPUpdate) == 32);
static_assert(sizeof(MBOUpdate) == 64);
static_assert(sizeof(Trade) == 64);
static_assert(sizeof(Fill) == 64);
static_assert(sizeof(Statistics) == 32);
static_assert(sizeof(Measurement) == 16);

}  // namespace roq
