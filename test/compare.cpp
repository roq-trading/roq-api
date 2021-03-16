/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include "roq/compare.h"

namespace roq {
namespace {
static_assert(!detail::isnan(0.0));
static_assert(detail::isnan(std::numeric_limits<double>::quiet_NaN()));

static_assert(detail::fabs(0.0) == 0.0);
static_assert(detail::fabs(-0.0) == 0.0);
static_assert(detail::fabs(1.0) == 1.0);
static_assert(detail::fabs(-1.0) == 1.0);

static_assert(compare(0.0, 0.0) == 0);
static_assert(compare(1.0, -1.0) == 1);
static_assert(compare(-1.0, 1.0) == -1);

static_assert(compare(NaN, NaN) == 0);
static_assert(compare(0.0, NaN) == 1);
static_assert(compare(NaN, 0.0) == -1);
}  // namespace
}  // namespace roq
