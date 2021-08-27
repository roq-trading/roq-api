/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include "roq/api.h"
#include "roq/metrics.h"

#include "roq/client/api.h"

namespace roq {
namespace client {

//! Client interface used to implement handlers for update events
/*!
 * It is the responsibility of the client implementation
 * to manage state and state transitions.
 *
 * The client is not allowed to throw. Any exception
 * thrown by the event handler may result in termination
 * of the process or, at least, undefined behaviour.
 */
class ROQ_PUBLIC Handler {
 public:
  // host
  virtual void operator()(const Event<Start> &) {}
  virtual void operator()(const Event<Stop> &) {}
  virtual void operator()(const Event<Timer> &) {}
  virtual void operator()(const Event<Connected> &) {}
  virtual void operator()(const Event<Disconnected> &) {}

  // control
  virtual void operator()(const Event<BatchBegin> &) {}
  virtual void operator()(const Event<BatchEnd> &) {}
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

  // broadcast
  virtual void operator()(const Event<CustomMetricsUpdate> &) {}

  // ancillary
  virtual void operator()(const Event<CustomMessage> &) {}

  // metrics
  virtual void operator()(metrics::Writer &) const {}
};

}  // namespace client
}  // namespace roq
