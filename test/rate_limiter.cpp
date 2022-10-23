/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include "roq/oms/rate_limiter.hpp"

using namespace std::literals;
using namespace std::chrono_literals;  // NOLINT

using namespace roq;

namespace {
auto dispatch_timer(auto &rate_limiter, auto now) {
  MessageInfo message_info;
  message_info.receive_time = now;
  Timer timer{
      .now = now,
  };
  Event event{message_info, timer};
  return rate_limiter(event);
}

auto dispatch_message_info(auto &rate_limiter, auto receive_time) {
  MessageInfo message_info;
  message_info.receive_time = receive_time;
  CreateOrder create_order;
  Event event{message_info, create_order};
  return rate_limiter(event);
}
}  // namespace

TEST_CASE("rate_limiter_simple", "[rate_limiter]") {
  struct Handler final : public oms::RateLimiter::Handler {
    void operator()(RateLimitTrigger const &) override { ++counter; }
    size_t counter = {};
  } handler;
  oms::RateLimiter::Config config{
      .name = "test"s,
      .type = RateLimitType::CREATE_ORDER,
      .aggregate = false,
      .request_limit = 10,
      .monitor_period = 10s,
      .ban_period = 10s,
      .low_water_mark = {},
      .high_water_mark = {},
  };
  oms::RateLimiter rate_limiter{handler, config};
  for (size_t i = 0; i < 20; ++i)
    CHECK(dispatch_message_info(rate_limiter, std::chrono::seconds{i}) == false);
  CHECK(handler.counter == 0);
}

TEST_CASE("rate_limiter_repeat_offender", "[rate_limiter]") {
  struct Handler final : public oms::RateLimiter::Handler {
    void operator()(RateLimitTrigger const &) override { ++counter; }
    size_t counter = {};
  } handler;
  oms::RateLimiter::Config config{
      .name = "test"s,
      .type = RateLimitType::CREATE_ORDER,
      .aggregate = false,
      .request_limit = 10,
      .monitor_period = 20s,
      .ban_period = 20s,
      .low_water_mark = {},
      .high_water_mark = {},
  };
  oms::RateLimiter rate_limiter{handler, config};
  for (size_t i = 0; i < 20; ++i)
    if (i < 10) {
      CHECK(dispatch_message_info(rate_limiter, std::chrono::seconds{i}) == false);
    } else {
      CHECK(dispatch_message_info(rate_limiter, std::chrono::seconds{i}) == true);
      CHECK(handler.counter == (i - 9));
    }
}

TEST_CASE("rate_limiter_good_citizen", "[rate_limiter]") {
  struct Handler final : public oms::RateLimiter::Handler {
    void operator()(RateLimitTrigger const &) override { ++counter; }
    size_t counter = {};
  } handler;
  oms::RateLimiter::Config config{
      .name = "test"s,
      .type = RateLimitType::CREATE_ORDER,
      .aggregate = false,
      .request_limit = 10,
      .monitor_period = 20s,
      .ban_period = 20s,
      .low_water_mark = {},
      .high_water_mark = {},
  };
  oms::RateLimiter rate_limiter{handler, config};
  for (size_t i = 0; i < 20; ++i)
    if (i < 10) {
      CHECK(dispatch_message_info(rate_limiter, std::chrono::seconds{i}) == false);
    } else if (i == 11) {
      CHECK(dispatch_message_info(rate_limiter, std::chrono::seconds{i}) == true);
    } else {
      CHECK(dispatch_message_info(rate_limiter, std::chrono::seconds{i + 20}) == false);
    }
}

TEST_CASE("rate_limiter_watermark_requests", "[rate_limiter]") {
  struct Handler final : public oms::RateLimiter::Handler {
    void operator()(RateLimitTrigger const &rate_limit_trigger) override {
      ++counter;
      switch (rate_limit_trigger.buffer_capacity) {
        using enum BufferCapacity;
        case UNDEFINED:
          break;
        case EMPTY:
          ++empty;
          break;
        case LOW_WATER_MARK:
          ++low;
          CHECK(rate_limit_trigger.remaining_requests == 5);
          break;
        case HIGH_WATER_MARK:
          ++high;
          CHECK(rate_limit_trigger.remaining_requests == 2);
          break;
        case FULL:
          ++full;
          CHECK(rate_limit_trigger.remaining_requests == 0);
          break;
      }
    }
    size_t counter = {};
    size_t empty = {};
    size_t low = {};
    size_t high = {};
    size_t full = {};
  } handler;
  oms::RateLimiter::Config config{
      .name = "test"s,
      .type = RateLimitType::CREATE_ORDER,
      .aggregate = false,
      .request_limit = 10,
      .monitor_period = 10s,
      .ban_period = 10s,
      .low_water_mark = 5,
      .high_water_mark = 8,
  };
  oms::RateLimiter rate_limiter{handler, config};
  for (size_t i = 0; i < 20; ++i)
    if (i < 7) {
      CHECK(dispatch_message_info(rate_limiter, std::chrono::seconds{i}) == false);
      CHECK(handler.empty == 0);
      CHECK(handler.low == 0);
      CHECK(handler.high == 0);
      CHECK(handler.full == 0);
    } else if (i < 10) {
      CHECK(dispatch_message_info(rate_limiter, std::chrono::seconds{i}) == false);
      CHECK(handler.empty == 0);
      CHECK(handler.low == 0);
      CHECK(handler.high == 1);
      CHECK(handler.full == 0);
    } else if (i < 11) {
      CHECK(dispatch_message_info(rate_limiter, std::chrono::seconds{10}) == false);
      CHECK(handler.empty == 0);
      CHECK(handler.low == 0);
      CHECK(handler.high == 1);
      CHECK(handler.full == 0);
    } else if (i < 12) {
      CHECK(dispatch_message_info(rate_limiter, std::chrono::seconds{10}) == true);
      CHECK(handler.empty == 0);
      CHECK(handler.low == 0);
      CHECK(handler.high == 1);
      CHECK(handler.full == 1);
    } else if (i < 13) {
      // note! must wait ban period
      CHECK(dispatch_message_info(rate_limiter, std::chrono::seconds{21}) == false);
      CHECK(handler.empty == 0);
      CHECK(handler.low == 1);
      CHECK(handler.high == 1);
      CHECK(handler.full == 1);
    } else if (i < 19) {
      CHECK(dispatch_message_info(rate_limiter, std::chrono::seconds{21}) == false);
      CHECK(handler.empty == 0);
      CHECK(handler.low == 1);
      CHECK(handler.high == 1);
      CHECK(handler.full == 1);
    } else if (i < 20) {
      CHECK(dispatch_message_info(rate_limiter, std::chrono::seconds{21}) == false);
      CHECK(handler.empty == 0);
      CHECK(handler.low == 1);
      CHECK(handler.high == 2);
      CHECK(handler.full == 1);
    }
}

TEST_CASE("rate_limiter_watermark_timer", "[rate_limiter]") {
  struct Handler final : public oms::RateLimiter::Handler {
    void operator()(RateLimitTrigger const &rate_limit_trigger) override {
      ++counter;
      switch (rate_limit_trigger.buffer_capacity) {
        using enum BufferCapacity;
        case UNDEFINED:
          break;
        case EMPTY:
          ++empty;
          break;
        case LOW_WATER_MARK:
          ++low;
          CHECK(rate_limit_trigger.remaining_requests == 5);
          break;
        case HIGH_WATER_MARK:
          ++high;
          CHECK(rate_limit_trigger.remaining_requests == 2);
          break;
        case FULL:
          ++full;
          CHECK(rate_limit_trigger.remaining_requests == 0);
          break;
      }
    }
    size_t counter = {};
    size_t empty = {};
    size_t low = {};
    size_t high = {};
    size_t full = {};
  } handler;
  oms::RateLimiter::Config config{
      .name = "test"s,
      .type = RateLimitType::CREATE_ORDER,
      .aggregate = false,
      .request_limit = 10,
      .monitor_period = 10s,
      .ban_period = 10s,
      .low_water_mark = 5,
      .high_water_mark = 8,
  };
  oms::RateLimiter rate_limiter{handler, config};
  for (size_t i = 0; i < 13; ++i)
    if (i < 7) {
      CHECK(dispatch_message_info(rate_limiter, std::chrono::seconds{i}) == false);
      CHECK(handler.empty == 0);
      CHECK(handler.low == 0);
      CHECK(handler.high == 0);
      CHECK(handler.full == 0);
    } else if (i < 10) {
      CHECK(dispatch_message_info(rate_limiter, std::chrono::seconds{i}) == false);
      CHECK(handler.empty == 0);
      CHECK(handler.low == 0);
      CHECK(handler.high == 1);
      CHECK(handler.full == 0);
    } else if (i < 11) {
      CHECK(dispatch_message_info(rate_limiter, std::chrono::seconds{10}) == false);
      CHECK(handler.empty == 0);
      CHECK(handler.low == 0);
      CHECK(handler.high == 1);
      CHECK(handler.full == 0);
    } else if (i < 12) {
      CHECK(dispatch_message_info(rate_limiter, std::chrono::seconds{10}) == true);
      CHECK(handler.empty == 0);
      CHECK(handler.low == 0);
      CHECK(handler.high == 1);
      CHECK(handler.full == 1);
    } else if (i < 13) {
      // note! must wait ban period
      dispatch_timer(rate_limiter, std::chrono::seconds{21});
      CHECK(handler.empty == 0);
      CHECK(handler.low == 1);
      CHECK(handler.high == 1);
      CHECK(handler.full == 1);
    }
}
