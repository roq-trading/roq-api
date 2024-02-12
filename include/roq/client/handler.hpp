/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include "roq/compat.hpp"

#include "roq/api.hpp"

#include "roq/metrics/writer.hpp"

#include "roq/client/custom_message.hpp"

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
struct ROQ_PUBLIC Handler {
  // host
  virtual void operator()(Event<Start> const &) {}
  virtual void operator()(Event<Stop> const &) {}
  virtual void operator()(Event<Timer> const &) {}
  virtual void operator()(Event<Connected> const &) {}
  virtual void operator()(Event<Disconnected> const &) {}

  // control
  virtual void operator()(Event<BatchBegin> const &) {}
  virtual void operator()(Event<BatchEnd> const &) {}
  virtual void operator()(Event<DownloadBegin> const &) {}
  virtual void operator()(Event<DownloadEnd> const &) {}
  virtual void operator()(Event<Ready> const &) {}

  // config
  virtual void operator()(Event<GatewaySettings> const &) {}

  // stream
  virtual void operator()(Event<StreamStatus> const &) {}
  virtual void operator()(Event<ExternalLatency> const &) {}
  virtual void operator()(Event<RateLimitsUpdate> const &) {}
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

  // account management (exchange)
  virtual void operator()(Event<PositionUpdate> const &) {}
  virtual void operator()(Event<FundsUpdate> const &) {}

  // broadcast
  virtual void operator()(Event<CustomMetricsUpdate> const &) {}
  virtual void operator()(Event<CustomMatrixUpdate> const &) {}

  // ancillary
  virtual void operator()(Event<CustomMessage> const &) {}

  // parameters
  virtual void operator()(Event<ParametersUpdate> const &) {}

  // position manager (service)
  virtual void operator()(Event<PortfolioUpdate> const &) {}

  // risk managemenet
  virtual void operator()(Event<RiskLimitsUpdate> const &) {}

  // metrics
  virtual void operator()(metrics::Writer &) const {}
};

}  // namespace client
}  // namespace roq
