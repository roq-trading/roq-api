/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include "roq/api.h"

namespace roq {


// interface used to implement order matching

class ROQ_PUBLIC Matcher {
 public:
  class ROQ_PUBLIC Dispatcher {
   public:
    virtual void operator()(const Event<DownloadBegin>&) = 0;
    virtual void operator()(const Event<DownloadEnd>&) = 0;
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
    virtual void operator()(const Event<OrderAck>&) = 0;
    virtual void operator()(const Event<OrderUpdate>&) = 0;
    virtual void operator()(const Event<TradeUpdate>&) = 0;
    virtual void operator()(const Event<PositionUpdate>&) = 0;
    virtual void operator()(const Event<FundsUpdate>&) = 0;
  };

  virtual ~Matcher() {}

  virtual void dispatch(
      Dispatcher& dispatcher,
      std::chrono::nanoseconds next) = 0;

  virtual void operator()(const Event<MarketDataStatus>&) = 0;
  virtual void operator()(const Event<OrderManagerStatus>&) = 0;
  virtual void operator()(const Event<ReferenceData>&) = 0;
  virtual void operator()(const Event<MarketStatus>&) = 0;
  virtual void operator()(const Event<SessionStatistics>&) = 0;
  virtual void operator()(const Event<DailyStatistics>&) = 0;
  virtual void operator()(const Event<TopOfBook>&) = 0;
  virtual void operator()(const Event<MarketByPriceUpdate>&) = 0;
  virtual void operator()(const Event<MarketByOrderUpdate>&) = 0;
  virtual void operator()(const Event<TradeSummary>&) = 0;
  virtual void operator()(const Event<CreateOrder>&) = 0;
  virtual void operator()(const Event<ModifyOrder>&) = 0;
  virtual void operator()(const Event<CancelOrder>&) = 0;
  virtual void operator()(const Event<PositionUpdate>&) = 0;
  virtual void operator()(const Event<FundsUpdate>&) = 0;
};

}  // namespace roq
