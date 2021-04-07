/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include "roq/api.h"

namespace roq {
namespace client {

//! Collector interface
class Collector {
 public:
  virtual ~Collector() {}

  virtual void operator()(const Event<DownloadBegin> &) {}
  virtual void operator()(const Event<DownloadEnd> &) {}
  virtual void operator()(const Event<GatewaySettings> &) {}
  virtual void operator()(const Event<GatewayStatus> &) {}
  virtual void operator()(const Event<ExternalLatency> &) {}
  virtual void operator()(const Event<ReferenceData> &) {}
  virtual void operator()(const Event<MarketStatus> &) {}
  virtual void operator()(const Event<TopOfBook> &) {}
  virtual void operator()(const Event<MarketByPriceUpdate> &) {}
  virtual void operator()(const Event<MarketByOrderUpdate> &) {}
  virtual void operator()(const Event<TradeSummary> &) {}
  virtual void operator()(const Event<StatisticsUpdate> &) {}
  virtual void operator()(const Event<CreateOrder> &) {}
  virtual void operator()(const Event<ModifyOrder> &) {}
  virtual void operator()(const Event<CancelOrder> &) {}
  virtual void operator()(const Event<OrderAck> &) {}
  virtual void operator()(const Event<OrderUpdate> &) {}
  virtual void operator()(const Event<TradeUpdate> &) {}
  virtual void operator()(const Event<PositionUpdate> &) {}
  virtual void operator()(const Event<FundsUpdate> &) {}
};

}  // namespace client
}  // namespace roq
