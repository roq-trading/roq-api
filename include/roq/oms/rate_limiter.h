/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include "roq/api.h"

namespace roq {
namespace oms {

//! Rate limiter interface (roq-client provides the factory)
struct ROQ_PUBLIC RateLimiter {
  struct Handler {
    virtual void operator()(const RateLimitTrigger &) = 0;
  };

  virtual ~RateLimiter() {}

  virtual bool operator()(const Event<CreateOrder> &) = 0;
  virtual bool operator()(const Event<ModifyOrder> &) = 0;
  virtual bool operator()(const Event<CancelOrder> &) = 0;
};

}  // namespace oms
}  // namespace roq
