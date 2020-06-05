/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include "roq/api.h"

namespace roq {


// interface used to implement order matching

class ROQ_PUBLIC Matcher {
 public:
  class ROQ_PUBLIC Dispatcher {
   public:
    virtual void operator()(const DownloadBeginEvent&) = 0;
    virtual void operator()(const DownloadEndEvent&) = 0;
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
    virtual void operator()(const OrderAckEvent&) = 0;
    virtual void operator()(const OrderUpdateEvent&) = 0;
    virtual void operator()(const TradeUpdateEvent&) = 0;
    virtual void operator()(const PositionUpdateEvent&) = 0;
    virtual void operator()(const FundsUpdateEvent&) = 0;
  };

  virtual ~Matcher() {}

  virtual void dispatch(
      Dispatcher& dispatcher,
      std::chrono::nanoseconds next) = 0;

  virtual void operator()(const MarketDataStatusEvent&) = 0;
  virtual void operator()(const OrderManagerStatusEvent&) = 0;
  virtual void operator()(const ReferenceDataEvent&) = 0;
  virtual void operator()(const MarketStatusEvent&) = 0;
  virtual void operator()(const SessionStatisticsEvent&) = 0;
  virtual void operator()(const DailyStatisticsEvent&) = 0;
  virtual void operator()(const TopOfBookEvent&) = 0;
  virtual void operator()(const MarketByPriceEvent&) = 0;
  virtual void operator()(const MarketByOrderEvent&) = 0;
  virtual void operator()(const TradeSummaryEvent&) = 0;
  virtual void operator()(const CreateOrderEvent&) = 0;
  virtual void operator()(const ModifyOrderEvent&) = 0;
  virtual void operator()(const CancelOrderEvent&) = 0;
  virtual void operator()(const PositionUpdateEvent&) = 0;
  virtual void operator()(const FundsUpdateEvent&) = 0;
};

}  // namespace roq
