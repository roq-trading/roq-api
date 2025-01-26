/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#include "roq/compat.hpp"

// convenience

#include "roq/limits.hpp"

// enums

#include "roq/action.hpp"
#include "roq/buffer_capacity.hpp"
#include "roq/category.hpp"
#include "roq/connection_status.hpp"
#include "roq/encoding.hpp"
#include "roq/error.hpp"
#include "roq/execution_instruction.hpp"
#include "roq/filter.hpp"
#include "roq/liquidity.hpp"
#include "roq/margin_mode.hpp"
#include "roq/option_type.hpp"
#include "roq/order_cancel_policy.hpp"
#include "roq/order_management.hpp"
#include "roq/order_status.hpp"
#include "roq/order_type.hpp"
#include "roq/origin.hpp"
#include "roq/position_effect.hpp"
#include "roq/precision.hpp"
#include "roq/priority.hpp"
#include "roq/protocol.hpp"
#include "roq/quantity_type.hpp"
#include "roq/rate_limit_type.hpp"
#include "roq/request_id_type.hpp"
#include "roq/request_status.hpp"
#include "roq/request_type.hpp"
#include "roq/route_request_status.hpp"
#include "roq/security_type.hpp"
#include "roq/side.hpp"
#include "roq/state.hpp"
#include "roq/statistics_type.hpp"
#include "roq/support_type.hpp"
#include "roq/time_in_force.hpp"
#include "roq/trading_status.hpp"
#include "roq/transport.hpp"
#include "roq/update_action.hpp"
#include "roq/update_reason.hpp"
#include "roq/update_type.hpp"

// helpers

#include "roq/bar.hpp"
#include "roq/fill.hpp"
#include "roq/layer.hpp"
#include "roq/leg.hpp"
#include "roq/mbo_update.hpp"
#include "roq/mbp_update.hpp"
#include "roq/measurement.hpp"
#include "roq/parameter.hpp"
#include "roq/position.hpp"
#include "roq/quote.hpp"
#include "roq/rate_limit.hpp"
#include "roq/risk_limit.hpp"
#include "roq/route.hpp"
#include "roq/statistics.hpp"
#include "roq/trade.hpp"

// transport

#include "roq/message_info.hpp"

// control (autogen)

#include "roq/connected.hpp"
#include "roq/control.hpp"
#include "roq/disconnected.hpp"
#include "roq/service_update.hpp"
#include "roq/start.hpp"
#include "roq/stop.hpp"
#include "roq/strategy_update.hpp"
#include "roq/timer.hpp"

// messages (autogen)

#include "roq/add_routes.hpp"
#include "roq/batch_begin.hpp"
#include "roq/batch_end.hpp"
#include "roq/cancel_all_orders.hpp"
#include "roq/cancel_all_orders_ack.hpp"
#include "roq/cancel_order.hpp"
#include "roq/cancel_quotes.hpp"
#include "roq/cancel_quotes_ack.hpp"
#include "roq/create_order.hpp"
#include "roq/custom_matrix.hpp"
#include "roq/custom_matrix_update.hpp"
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
#include "roq/mass_quote.hpp"
#include "roq/mass_quote_ack.hpp"
#include "roq/modify_order.hpp"
#include "roq/order_ack.hpp"
#include "roq/order_update.hpp"
#include "roq/parameters_update.hpp"
#include "roq/portfolio.hpp"
#include "roq/portfolio_update.hpp"
#include "roq/position_update.hpp"
#include "roq/rate_limit_trigger.hpp"
#include "roq/rate_limits_update.hpp"
#include "roq/ready.hpp"
#include "roq/reference_data.hpp"
#include "roq/remove_routes.hpp"
#include "roq/risk_limits.hpp"
#include "roq/risk_limits_update.hpp"
#include "roq/route_ack.hpp"
#include "roq/statistics_update.hpp"
#include "roq/stream_status.hpp"
#include "roq/time_series_update.hpp"
#include "roq/top_of_book.hpp"
#include "roq/trade_summary.hpp"
#include "roq/trade_update.hpp"

// misc

#include "roq/exceptions.hpp"

// version

#define ROQ_VERSION "1.0.4"

namespace roq {

// user id (8 bits)

static constexpr uint8_t const SOURCE_MIN = 0;
static constexpr uint8_t const SOURCE_MAX = 255;

static constexpr auto const SOURCE_NONE = SOURCE_MIN;
static constexpr auto const SOURCE_SELF = SOURCE_MAX;

// stream id (16 bits)

static constexpr uint16_t const STREAM_ID_MIN = 0;
static constexpr uint16_t const STREAM_ID_MAX = 65535;

static constexpr auto const STREAM_ID_NONE = STREAM_ID_MIN;

// order id (48 bits)

static constexpr uint64_t const ORDER_ID_MIN = 0;
static constexpr uint64_t const ORDER_ID_MAX = (uint64_t{1} << 48) - 1;

static constexpr auto const ORDER_ID_NONE = ORDER_ID_MIN;

// request version (24 bits)

static constexpr uint32_t const REQUEST_VERSION_MIN = 0;
static constexpr uint32_t const REQUEST_VERSION_MAX = (uint32_t{1} << 24) - 1;

static constexpr auto const REQUEST_VERSION_NONE = REQUEST_VERSION_MIN;

// strategy id (24 bits)

static constexpr uint32_t const STRATEGY_ID_MIN = 0;
static constexpr uint32_t const STRATEGY_ID_MAX = (uint32_t{1} << 24) - 1;

static constexpr auto const STRATEGY_ID_NONE = STRATEGY_ID_MIN;

// account id (8 bits)

static constexpr uint8_t const ACCOUNT_MIN = 0;
static constexpr uint8_t const ACCOUNT_MAX = 255;

static constexpr auto const ACCOUNT_NONE = ACCOUNT_MIN;

// symbol id (24 bits)

static constexpr uint32_t const SYMBOL_ID_MIN = 0;
static constexpr uint32_t const SYMBOL_ID_MAX = (uint32_t{1} << 24) - 1;

static constexpr auto const SYMBOL_ID_NONE = SYMBOL_ID_MIN;

// instance id (4 bits)

static constexpr uint8_t const INSTANCE_MIN = 0;
static constexpr uint8_t const INSTANCE_MAX = (uint8_t{1} << 4) - 1;

static constexpr auto const INSTANCE_NONE = INSTANCE_MIN;

// validate auto-generated code

static_assert(sizeof(decltype(RateLimitTrigger::users)::value_type) == sizeof(User));
static_assert(sizeof(decltype(RateLimitTrigger::accounts)::value_type) == sizeof(Account));

static_assert(sizeof(decltype(Measurement::name)) == sizeof(MeasurementKey));

// check size of certain array items
// - frequent -- should be (reasonably) cache aligned
static_assert(sizeof(Layer) == 32);
static_assert(sizeof(MBPUpdate) == 32);
static_assert(sizeof(MBOUpdate) == 64);
static_assert(sizeof(Trade) == 192);
static_assert(sizeof(Statistics) == 32);
static_assert(sizeof(Measurement) == 16);
static_assert(sizeof(Route) == 8);
static_assert(sizeof(TickSizeStep) == 16);
// - not frequent -- not so important
static_assert(sizeof(Fill) == 120);
static_assert(sizeof(Parameter) == 180);
static_assert(sizeof(Position) == 96);
static_assert(sizeof(RiskLimit) == 136);

}  // namespace roq
