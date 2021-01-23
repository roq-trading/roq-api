/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <utility>

#include "roq/api.h"

namespace roq {

//! Generator interface used to manage a sequence of messages
class ROQ_PUBLIC Generator {
 public:
  //! Dispatch interface used to handle update events
  class ROQ_PUBLIC Dispatcher {
   public:
    virtual void operator()(const Event<MarketDataStatus> &) = 0;
    virtual void operator()(const Event<OrderManagerStatus> &) = 0;
    virtual void operator()(const Event<ReferenceData> &) = 0;
    virtual void operator()(const Event<MarketStatus> &) = 0;
    virtual void operator()(const Event<TopOfBook> &) = 0;
    virtual void operator()(const Event<MarketByPriceUpdate> &) = 0;
    virtual void operator()(const Event<MarketByOrderUpdate> &) = 0;
    virtual void operator()(const Event<TradeSummary> &) = 0;
    virtual void operator()(const Event<StatisticsUpdate> &) = 0;
    virtual void operator()(const Event<PositionUpdate> &) = 0;
    virtual void operator()(const Event<FundsUpdate> &) = 0;
  };

  virtual ~Generator() {}

  //! Fetch information about the next message
  virtual std::pair<bool, std::chrono::nanoseconds> fetch() = 0;

  //! Request the next message to be dispatched
  virtual void dispatch(Dispatcher &dispatcher) = 0;
};

}  // namespace roq
