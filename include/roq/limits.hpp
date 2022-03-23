/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <cstddef>
#include <cstdint>

namespace roq {
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
}  // namespace roq
