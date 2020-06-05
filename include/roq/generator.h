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
    virtual void operator()(const MarketDataStatusEvent&) = 0;
    virtual void operator()(const OrderManagerStatusEvent&) = 0;
    virtual void operator()(const ReferenceDataEvent&) = 0;
    virtual void operator()(const MarketStatusEvent&) = 0;
    virtual void operator()(const TopOfBookEvent&) = 0;
    virtual void operator()(const MarketByPriceEvent&) = 0;
    virtual void operator()(const MarketByOrderEvent&) = 0;
    virtual void operator()(const TradeSummaryEvent&) = 0;
    virtual void operator()(const SessionStatisticsEvent&) = 0;
    virtual void operator()(const DailyStatisticsEvent&) = 0;
    virtual void operator()(const PositionUpdateEvent&) = 0;
    virtual void operator()(const FundsUpdateEvent&) = 0;
  };

  virtual ~Generator() {}
  virtual std::pair<bool, std::chrono::nanoseconds> fetch() = 0;
  virtual void dispatch(Dispatcher& dispatcher) = 0;
};

}  // namespace roq
