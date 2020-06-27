/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <utility>

#include "roq/api.h"

namespace roq {

// Interface used to implement an event generator.

class ROQ_PUBLIC Generator {
 public:
  class ROQ_PUBLIC Dispatcher {
   public:
    virtual void operator()(const Event<MarketDataStatus>&) = 0;
    virtual void operator()(const Event<OrderManagerStatus>&) = 0;
    virtual void operator()(const Event<ReferenceData>&) = 0;
    virtual void operator()(const Event<MarketStatus>&) = 0;
    virtual void operator()(const Event<TopOfBook>&) = 0;
    virtual void operator()(const Event<MarketByPriceUpdate>&) = 0;
    virtual void operator()(const Event<MarketByOrderUpdate>&) = 0;
    virtual void operator()(const Event<TradeSummary>&) = 0;
    virtual void operator()(const Event<SessionStatistics>&) = 0;
    virtual void operator()(const Event<DailyStatistics>&) = 0;
    virtual void operator()(const Event<PositionUpdate>&) = 0;
    virtual void operator()(const Event<FundsUpdate>&) = 0;
  };

  virtual ~Generator() {}
  virtual std::pair<bool, std::chrono::nanoseconds> fetch() = 0;
  virtual void dispatch(Dispatcher& dispatcher) = 0;
};

}  // namespace roq
