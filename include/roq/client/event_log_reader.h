/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.h"

namespace roq {
namespace client {

class ROQ_PUBLIC EventLogReader {
 public:
  struct Handler {
    // config
    virtual void operator()(const Event<GatewaySettings> &) = 0;

    // stream
    virtual void operator()(const Event<StreamStatus> &) = 0;
    virtual void operator()(const Event<ExternalLatency> &) = 0;
    virtual void operator()(const Event<RateLimitTrigger> &) = 0;

    // service
    virtual void operator()(const Event<GatewayStatus> &) = 0;

    // market data
    virtual void operator()(const Event<ReferenceData> &) = 0;
    virtual void operator()(const Event<MarketStatus> &) = 0;
    virtual void operator()(const Event<TopOfBook> &) = 0;
    virtual void operator()(const Event<MarketByPriceUpdate> &) = 0;
    virtual void operator()(const Event<MarketByOrderUpdate> &) = 0;
    virtual void operator()(const Event<TradeSummary> &) = 0;
    virtual void operator()(const Event<StatisticsUpdate> &) = 0;

    // order actions (from client, note! source == user)
    virtual void operator()(const Event<CreateOrder> &) = 0;
    virtual void operator()(const Event<ModifyOrder> &) = 0;
    virtual void operator()(const Event<CancelOrder> &) = 0;
    virtual void operator()(const Event<CancelAllOrders> &) = 0;

    // order management
    virtual void operator()(const Event<OrderAck> &) = 0;
    virtual void operator()(const Event<OrderUpdate> &) = 0;
    virtual void operator()(const Event<TradeUpdate> &) = 0;

    // account management
    virtual void operator()(const Event<PositionUpdate> &) = 0;
    virtual void operator()(const Event<FundsUpdate> &) = 0;

    // broadcast
    virtual void operator()(const Event<CustomMetricsUpdate> &) = 0;
  };

  virtual ~EventLogReader() {}

  // all events have been dispatched and the producer has closed the file properly
  // note! this state may never be reached if the producer did not terminate normally
  // note! observing a "newer" file can be used to positively decide that this file has "finished"
  virtual bool finished() = 0;

  // returns true when one or more events have been dispatched
  // returns false when there are no more data available
  // note! false could mean either the producer is buffering or the file was indeed closed
  virtual bool dispatch(Handler &) = 0;
};

}  // namespace client
}  // namespace roq
