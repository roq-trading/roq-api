/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include "roq/api.h"
#include "roq/metrics.h"

#include "roq/client/api.h"

namespace roq {
namespace client {

/* Interface used by a client implementation to handle
 * update events.
 *
 * It is the responsibility of the client implementation
 * to manage state and state transitions.
 *
 * The client is not allowed to throw. Any exception
 * thrown by the event handler may result in termination
 * of the process or, at least, undefined behaviour.
 */
class ROQ_PUBLIC Handler {
 public:
  virtual void operator()(const StartEvent&) {}
  virtual void operator()(const StopEvent&) {}
  virtual void operator()(const TimerEvent&) {}

  // connection
  virtual void operator()(const Event<ConnectionStatus>&) {}

  // messages
  virtual void operator()(const Event<BatchBegin>&) {}
  virtual void operator()(const Event<BatchEnd>&) {}
  virtual void operator()(const Event<DownloadBegin>&) {}
  virtual void operator()(const Event<DownloadEnd>&) {}
  virtual void operator()(const Event<MarketDataStatus>&) {}
  virtual void operator()(const Event<OrderManagerStatus>&) {}
  virtual void operator()(const Event<ReferenceData>&) {}
  virtual void operator()(const Event<MarketStatus>&) {}
  virtual void operator()(const Event<TopOfBook>&) {}
  virtual void operator()(const Event<MarketByPriceUpdate>&) {}
  virtual void operator()(const Event<MarketByOrderUpdate>&) {}
  virtual void operator()(const Event<TradeSummary>&) {}
  virtual void operator()(const Event<SessionStatistics>&) {}
  virtual void operator()(const Event<DailyStatistics>&) {}
  virtual void operator()(const Event<OrderAck>&) {}
  virtual void operator()(const Event<OrderUpdate>&) {}
  virtual void operator()(const Event<TradeUpdate>&) {}
  virtual void operator()(const Event<PositionUpdate>&) {}
  virtual void operator()(const Event<FundsUpdate>&) {}
  virtual void operator()(const Event<CustomMessage>&) {}

  // metrics
  virtual void operator()(metrics::Writer&) const {}
};

}  // namespace client
}  // namespace roq
