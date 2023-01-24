/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include "roq/compat.hpp"

// convenience

#include "roq/numbers.hpp"

// enums

#include "roq/buffer_capacity.hpp"
#include "roq/category.hpp"
#include "roq/connection_status.hpp"
#include "roq/decimals.hpp"
#include "roq/encoding.hpp"
#include "roq/error.hpp"
#include "roq/execution_instruction.hpp"
#include "roq/liquidity.hpp"
#include "roq/option_type.hpp"
#include "roq/order_cancel_policy.hpp"
#include "roq/order_management.hpp"
#include "roq/order_status.hpp"
#include "roq/order_type.hpp"
#include "roq/origin.hpp"
#include "roq/position_effect.hpp"
#include "roq/priority.hpp"
#include "roq/protocol.hpp"
#include "roq/rate_limit_type.hpp"
#include "roq/request_id_type.hpp"
#include "roq/request_status.hpp"
#include "roq/request_type.hpp"
#include "roq/security_type.hpp"
#include "roq/side.hpp"
#include "roq/statistics_type.hpp"
#include "roq/support_type.hpp"
#include "roq/time_in_force.hpp"
#include "roq/trading_status.hpp"
#include "roq/transport.hpp"
#include "roq/update_action.hpp"
#include "roq/update_type.hpp"

// helpers

#include "roq/fill.hpp"
#include "roq/layer.hpp"
#include "roq/mbo_update.hpp"
#include "roq/mbp_update.hpp"
#include "roq/measurement.hpp"
#include "roq/parameter.hpp"
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
#include "roq/parameters_update.hpp"
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

#define ROQ_VERSION "0.9.1"

namespace roq {

// user id's (restricted values)

static constexpr auto const SOURCE_NONE = uint8_t{0};
static constexpr auto const SOURCE_SELF = uint8_t{255};

// stream id's

static constexpr auto const STREAM_ID_NONE = uint16_t{0};

// order id's (restricted values and upper limit)

static constexpr auto const ORDER_ID_NONE = uint32_t{0};
static constexpr auto const MAX_ORDER_ID = (uint32_t{1} << 24) - 1;

// request versions (upper limit)

static constexpr auto const MAX_REQUEST_VERSION = (uint32_t{1} << 24) - 1;

// account id's (restricted values)

static constexpr auto const ACCOUNT_NONE = uint8_t{0};

// symbol id's (restricted values)

static constexpr auto const SYMBOL_ID_NONE = uint32_t{0};

// validate auto-generated code

static_assert(sizeof(decltype(RateLimitTrigger::users)::value_type) == sizeof(User));
static_assert(sizeof(decltype(RateLimitTrigger::accounts)::value_type) == sizeof(Account));

static_assert(sizeof(decltype(Measurement::name)) == sizeof(MeasurementKey));

// check that array items can be cache aligned

static_assert(sizeof(Layer) == 32);
static_assert(sizeof(MBPUpdate) == 32);
static_assert(sizeof(MBOUpdate) == 64);
static_assert(sizeof(Trade) == 192);
static_assert(sizeof(Fill) == 64);
static_assert(sizeof(Statistics) == 32);
static_assert(sizeof(Measurement) == 16);

// we don't care so much (low frequency) -- just keep a handle on the size

static_assert(sizeof(Parameter) == 176);

}  // namespace roq
