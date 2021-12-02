/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include "roq/api.h"

namespace roq {
namespace client {

//! Collector interface
class Collector {
 public:
  virtual ~Collector() {}

  // control
  virtual void operator()(const Event<DownloadBegin> &) {}
  virtual void operator()(const Event<DownloadEnd> &) {}

  // config
  virtual void operator()(const Event<GatewaySettings> &) {}

  // stream
  virtual void operator()(const Event<StreamStatus> &) {}
  virtual void operator()(const Event<ExternalLatency> &) {}
  virtual void operator()(const Event<RateLimitTrigger> &) {}

  // service
  virtual void operator()(const Event<GatewayStatus> &) {}

  // market data
  virtual void operator()(const Event<ReferenceData> &) {}
  virtual void operator()(const Event<MarketStatus> &) {}
  virtual void operator()(const Event<TopOfBook> &) {}
  virtual void operator()(const Event<MarketByPriceUpdate> &) {}
  virtual void operator()(const Event<MarketByOrderUpdate> &) {}
  virtual void operator()(const Event<TradeSummary> &) {}
  virtual void operator()(const Event<StatisticsUpdate> &) {}

  // order management
  virtual void operator()(const Event<OrderAck> &) {}
  virtual void operator()(const Event<OrderUpdate> &) {}
  virtual void operator()(const Event<TradeUpdate> &) {}

  // account management
  virtual void operator()(const Event<PositionUpdate> &) {}
  virtual void operator()(const Event<FundsUpdate> &) {}

  // client requests
  virtual void operator()(const Event<CreateOrder> &) {}
  virtual void operator()(const Event<ModifyOrder> &) {}
  virtual void operator()(const Event<CancelOrder> &) {}
  virtual void operator()(const Event<CancelAllOrders> &) {}

  // broadcast
  virtual void operator()(const Event<CustomMetricsUpdate> &) {}
};

}  // namespace client
}  // namespace roq
