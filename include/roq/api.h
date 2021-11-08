/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include "roq/compat.h"

// preparing for C++20

#include "roq/chrono.h"
#include "roq/span.h"

// convenience

#include "roq/numbers.h"

// enums

#include "roq/connection_status.h"
#include "roq/decimals.h"
#include "roq/error.h"
#include "roq/execution_instruction.h"
#include "roq/liquidity.h"
#include "roq/option_type.h"
#include "roq/order_cancel_policy.h"
#include "roq/order_management.h"
#include "roq/order_status.h"
#include "roq/order_type.h"
#include "roq/order_update_action.h"
#include "roq/origin.h"
#include "roq/position_effect.h"
#include "roq/priority.h"
#include "roq/rate_limit_type.h"
#include "roq/request_status.h"
#include "roq/request_type.h"
#include "roq/security_type.h"
#include "roq/side.h"
#include "roq/statistics_type.h"
#include "roq/stream_type.h"
#include "roq/support_type.h"
#include "roq/time_in_force.h"
#include "roq/trading_status.h"
#include "roq/update_type.h"

// helpers

#include "roq/fill.h"
#include "roq/layer.h"
#include "roq/mbo_update.h"
#include "roq/mbp_update.h"
#include "roq/measurement.h"
#include "roq/statistics.h"
#include "roq/trade.h"

// transport

#include "roq/message_info.h"

// control (autogen)

#include "roq/connected.h"
#include "roq/disconnected.h"
#include "roq/start.h"
#include "roq/stop.h"
#include "roq/timer.h"

// messages (autogen)

#include "roq/batch_begin.h"
#include "roq/batch_end.h"
#include "roq/cancel_all_orders.h"
#include "roq/cancel_order.h"
#include "roq/create_order.h"
#include "roq/custom_metrics.h"
#include "roq/custom_metrics_update.h"
#include "roq/download_begin.h"
#include "roq/download_end.h"
#include "roq/external_latency.h"
#include "roq/funds_update.h"
#include "roq/gateway_settings.h"
#include "roq/gateway_status.h"
#include "roq/market_by_order_update.h"
#include "roq/market_by_price_update.h"
#include "roq/market_status.h"
#include "roq/modify_order.h"
#include "roq/order_ack.h"
#include "roq/order_update.h"
#include "roq/position_update.h"
#include "roq/rate_limit_trigger.h"
#include "roq/reference_data.h"
#include "roq/statistics_update.h"
#include "roq/stream_status.h"
#include "roq/top_of_book.h"
#include "roq/trade_summary.h"
#include "roq/trade_update.h"

// misc

#include "roq/exceptions.h"

// version

#define ROQ_VERSION "0.7.8"

namespace roq {

// user id's (restricted)

static const constexpr uint8_t SOURCE_NONE = 0;
static const constexpr uint8_t SOURCE_SELF = 255;

// limits

static const constexpr size_t MAX_LENGTH_USER = 16;
static const constexpr size_t MAX_LENGTH_ACCOUNT = 32;
static const constexpr size_t MAX_LENGTH_EXCHANGE = 32;
static const constexpr size_t MAX_LENGTH_SYMBOL = 32;
static const constexpr size_t MAX_LENGTH_CURRENCY = 16;

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
