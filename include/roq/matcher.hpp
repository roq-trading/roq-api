/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

namespace roq {

//! Order matching interface used to manage a simulated order book
class ROQ_PUBLIC Matcher {
 public:
  //! Dispatch interface used to handle (possibly processed) events
  class ROQ_PUBLIC Dispatcher {
   public:
    virtual void operator()(Event<DownloadBegin> const &) = 0;
    virtual void operator()(Event<DownloadEnd> const &) = 0;

    // config
    virtual void operator()(Event<GatewaySettings> const &) = 0;

    // stream
    virtual void operator()(Event<StreamStatus> const &) = 0;

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

    // order management
    virtual void operator()(Event<OrderAck> const &) = 0;
    virtual void operator()(Event<OrderUpdate> const &) = 0;
    virtual void operator()(Event<TradeUpdate> const &) = 0;

    // account management
    virtual void operator()(Event<PositionUpdate> const &) = 0;
    virtual void operator()(Event<FundsUpdate> const &) = 0;
  };

  virtual ~Matcher() {}

  // config
  virtual void operator()(Event<GatewaySettings> const &) = 0;

  // stream
  virtual void operator()(Event<StreamStatus> const &) = 0;

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

  // account management
  virtual void operator()(Event<PositionUpdate> const &) = 0;
  virtual void operator()(Event<FundsUpdate> const &) = 0;

  // client request
  virtual void operator()(Event<CreateOrder> const &) = 0;
  virtual void operator()(Event<ModifyOrder> const &) = 0;
  virtual void operator()(Event<CancelOrder> const &) = 0;
  virtual void operator()(Event<CancelAllOrders> const &) = 0;
};

}  // namespace roq
