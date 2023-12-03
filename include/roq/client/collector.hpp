/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/api.hpp"

namespace roq {
namespace client {

//! Collector interface
struct Collector {
  virtual ~Collector() {}

  // control
  virtual void operator()(Event<DownloadBegin> const &) {}
  virtual void operator()(Event<DownloadEnd> const &) {}

  // config
  virtual void operator()(Event<GatewaySettings> const &) {}

  // stream
  virtual void operator()(Event<StreamStatus> const &) {}
  virtual void operator()(Event<ExternalLatency> const &) {}
  virtual void operator()(Event<RateLimitTrigger> const &) {}

  // service
  virtual void operator()(Event<GatewayStatus> const &) {}

  // market data
  virtual void operator()(Event<ReferenceData> const &) {}
  virtual void operator()(Event<MarketStatus> const &) {}
  virtual void operator()(Event<TopOfBook> const &) {}
  virtual void operator()(Event<MarketByPriceUpdate> const &) {}
  virtual void operator()(Event<MarketByOrderUpdate> const &) {}
  virtual void operator()(Event<TradeSummary> const &) {}
  virtual void operator()(Event<StatisticsUpdate> const &) {}

  // order management
  virtual void operator()(Event<CancelAllOrdersAck> const &) {}
  virtual void operator()(Event<OrderAck> const &) {}
  virtual void operator()(Event<OrderUpdate> const &) {}
  virtual void operator()(Event<TradeUpdate> const &) {}

  // account management
  virtual void operator()(Event<PositionUpdate> const &) {}
  virtual void operator()(Event<FundsUpdate> const &) {}

  // client requests
  virtual void operator()(Event<CreateOrder> const &) {}
  virtual void operator()(Event<ModifyOrder> const &) {}
  virtual void operator()(Event<CancelOrder> const &) {}
  virtual void operator()(Event<CancelAllOrders> const &) {}

  // broadcast
  virtual void operator()(Event<CustomMetricsUpdate> const &) {}
};

}  // namespace client
}  // namespace roq
