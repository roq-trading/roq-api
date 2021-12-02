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

  virtual bool finished() = 0;

  // returns false when done
  // note! handler is not guaranteed to be called on each invocation
  virtual bool dispatch(Handler &) = 0;
};

}  // namespace client
}  // namespace roq
