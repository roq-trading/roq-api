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
  virtual void operator()(const ConnectionStatusEvent&) {}
  virtual void operator()(const BatchBeginEvent&) {}
  virtual void operator()(const BatchEndEvent&) {}
  virtual void operator()(const DownloadBeginEvent&) {}
  virtual void operator()(const DownloadEndEvent&) {}
  virtual void operator()(const MarketDataStatusEvent&) {}
  virtual void operator()(const OrderManagerStatusEvent&) {}
  virtual void operator()(const ReferenceDataEvent&) {}
  virtual void operator()(const MarketStatusEvent&) {}
  virtual void operator()(const TopOfBookEvent&) {}
  virtual void operator()(const MarketByPriceEvent&) {}
  virtual void operator()(const MarketByOrderEvent&) {}
  virtual void operator()(const TradeSummaryEvent&) {}
  virtual void operator()(const SessionStatisticsEvent&) {}
  virtual void operator()(const DailyStatisticsEvent&) {}
  virtual void operator()(const OrderAckEvent&) {}
  virtual void operator()(const OrderUpdateEvent&) {}
  virtual void operator()(const TradeUpdateEvent&) {}
  virtual void operator()(const PositionUpdateEvent&) {}
  virtual void operator()(const FundsUpdateEvent&) {}
  virtual void operator()(const CustomMessageEvent&) {}

  virtual void operator()(metrics::Writer&) const {}
};

}  // namespace client
}  // namespace roq
