/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

namespace roq {
namespace client {

class ROQ_PUBLIC EventLogMultiplexer {
 public:
  struct Handler {
    // config
    virtual void operator()(Event<GatewaySettings> const &) = 0;

    // stream
    virtual void operator()(Event<StreamStatus> const &) = 0;
    virtual void operator()(Event<ExternalLatency> const &) = 0;
    virtual void operator()(Event<RateLimitTrigger> const &) = 0;

    // service
    virtual void operator()(Event<GatewayStatus> const &) = 0;

    // market data
    virtual void operator()(Event<ReferenceData> const &) = 0;
    virtual void operator()(Event<MarketStatus> const &) = 0;
    virtual void operator()(Event<TopOfBook> const &) = 0;
    virtual void operator()(Event<MarketByPriceUpdate> const &) = 0;
    virtual void operator()(Event<MarketByOrderUpdate> const &) = 0;
    virtual void operator()(Event<TradeSummary> const &) = 0;
    virtual void operator()(Event<StatisticsUpdate> const &) = 0;

    // order actions (from client, note! source == user)
    virtual void operator()(Event<CreateOrder> const &) = 0;
    virtual void operator()(Event<ModifyOrder> const &) = 0;
    virtual void operator()(Event<CancelOrder> const &) = 0;
    virtual void operator()(Event<CancelAllOrders> const &) = 0;

    // order management
    virtual void operator()(Event<OrderAck> const &) = 0;
    virtual void operator()(Event<OrderUpdate> const &) = 0;
    virtual void operator()(Event<TradeUpdate> const &) = 0;

    // account management
    virtual void operator()(Event<PositionUpdate> const &) = 0;
    virtual void operator()(Event<FundsUpdate> const &) = 0;

    // broadcast
    virtual void operator()(Event<CustomMetricsUpdate> const &) = 0;

    // parameters
    virtual void operator()(Event<ParameterUpdate> const &) = 0;
  };

  virtual ~EventLogMultiplexer() {}

  // returns true when one or more events have been dispatched
  // returns false when there are no more data available
  virtual bool dispatch(Handler &) = 0;
};

}  // namespace client
}  // namespace roq
