/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <deque>
#include <utility>
#include <vector>

#include "roq/api.hpp"

namespace roq {
namespace oms {

struct RateLimiter final {
  struct Handler {
    virtual void operator()(RateLimitTrigger const &) = 0;
  };

  struct Config final {
    std::string name;
    RateLimitType type;
    bool aggregate = false;
    uint32_t request_limit = {};
    std::chrono::nanoseconds monitor_period = {};
    std::chrono::nanoseconds ban_period = {};
    uint32_t low_water_mark = {};
    uint32_t high_water_mark = {};
  };

  RateLimiter(Handler &handler, Config &config) : handler_{handler}, config_{config} {}
  RateLimiter(Handler &handler, Config &config, std::vector<roq::Account> &&accounts, std::vector<roq::User> &&users)
      : handler_{handler}, config_{config}, accounts_{std::move(accounts)}, users_{std::move(users)} {}
  RateLimiter(
      Handler &handler, Config &config, std::vector<roq::Account> const &accounts, std::vector<roq::User> const &users)
      : handler_{handler}, config_{config}, accounts_{accounts}, users_{users} {}

  RateLimiter(RateLimiter &&) = default;
  RateLimiter(RateLimiter const &) = delete;

  void operator()(Event<Timer> const &event) { refresh(event.value.now); }

  bool operator()(Event<CreateOrder> const &event) { return update(event.message_info); }
  bool operator()(Event<ModifyOrder> const &event) {
    if (config_.type == RateLimitType::ORDER_ACTION)
      return update(event.message_info);
    return false;
  }
  bool operator()(Event<CancelOrder> const &event) {
    if (config_.type == RateLimitType::ORDER_ACTION)
      return update(event.message_info);
    return false;
  }

 protected:
  void refresh(std::chrono::nanoseconds now) {
    switch (capacity_) {
      using enum BufferCapacity;
      case UNDEFINED:
      case EMPTY:
      case LOW_WATER_MARK:
        break;
      case HIGH_WATER_MARK:
      case FULL:
        if (std::size(history_) >= config_.low_water_mark) {
          auto index = std::size(history_) - config_.low_water_mark;
          auto period = now - history_[index];
          if (period > config_.monitor_period) {
            capacity_ = LOW_WATER_MARK;
            auto remaining_requests = config_.request_limit - config_.low_water_mark;
            dispatch(remaining_requests);
          }
        }
        break;
    }
  }

  bool update(MessageInfo const &message_info) {
    auto receive_time = message_info.receive_time;
    auto reject = receive_time < ban_expires_;
    if (config_.request_limit == std::size(history_)) {
      auto period = receive_time - history_.front();
      if (period < config_.monitor_period) [[unlikely]] {
        capacity_ = BufferCapacity::FULL;
        ban_expires_ = receive_time + config_.ban_period;
        // XXX maybe only reject if buffer wasn't already full?
        reject = true;
        dispatch(0, message_info);
      }
      if (config_.request_limit <= std::size(history_)) [[likely]]
        history_.pop_front();
    }
    history_.push_back(receive_time);
    if (!reject && config_.low_water_mark && config_.high_water_mark) {
      switch (capacity_) {
        using enum BufferCapacity;
        case UNDEFINED:
        case EMPTY:
        case LOW_WATER_MARK:
          if (std::size(history_) >= config_.high_water_mark) {
            auto index = std::size(history_) - config_.high_water_mark;
            auto period = receive_time - history_[index];
            if (period < config_.monitor_period) {
              capacity_ = HIGH_WATER_MARK;
              auto remaining_requests = config_.request_limit - config_.high_water_mark;
              dispatch(remaining_requests, message_info);
            }
          }
          break;
        case HIGH_WATER_MARK:
        case FULL:
          if (std::size(history_) >= config_.low_water_mark) {
            auto index = std::size(history_) - config_.low_water_mark;
            auto period = receive_time - history_[index];
            if (period > config_.monitor_period) {
              capacity_ = LOW_WATER_MARK;
              auto remaining_requests = config_.request_limit - config_.low_water_mark;
              dispatch(remaining_requests, message_info);
            }
          }
          break;
      }
    }
    return reject;
  }

  void dispatch(uint32_t remaining_requests, MessageInfo const &message_info) {
    RateLimitTrigger rate_limit_trigger{
        .name = config_.name,
        .origin = Origin::GATEWAY,
        .type = config_.type,
        .users = users_,
        .accounts = accounts_,
        .ban_expires = ban_expires_,
        .triggered_by = message_info.source_name,
        .buffer_capacity = capacity_,
        .remaining_requests = remaining_requests,
    };
    handler_(rate_limit_trigger);
  }

  void dispatch(uint32_t remaining_requests) {
    RateLimitTrigger rate_limit_trigger{
        .name = config_.name,
        .origin = Origin::GATEWAY,
        .type = config_.type,
        .users = users_,
        .accounts = accounts_,
        .ban_expires = ban_expires_,
        .triggered_by = {},
        .buffer_capacity = capacity_,
        .remaining_requests = remaining_requests,
    };
    handler_(rate_limit_trigger);
  }

 public:
  Handler &handler_;
  Config const config_;
  std::vector<roq::Account> const accounts_;
  std::vector<roq::User> const users_;
  std::deque<std::chrono::nanoseconds> history_;
  std::chrono::nanoseconds ban_expires_ = {};
  BufferCapacity capacity_ = {};
};

}  // namespace oms
}  // namespace roq
