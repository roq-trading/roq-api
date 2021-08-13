/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <utility>

#include "roq/api.h"

namespace roq {

//! Generator interface used to manage a sequence of messages
class ROQ_PUBLIC Generator {
 public:
  //! Dispatch interface used to handle update events
  class ROQ_PUBLIC Dispatcher {
   public:
    // config
    virtual void operator()(const Event<GatewaySettings> &) = 0;

    // stream
    virtual void operator()(const Event<StreamStatus> &) = 0;

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

    // account management
    virtual void operator()(const Event<PositionUpdate> &) = 0;
    virtual void operator()(const Event<FundsUpdate> &) = 0;
  };

  virtual ~Generator() {}

  virtual uint8_t get_source_id() const = 0;
  virtual std::string_view get_source_name() const = 0;

  //! Peek next available message, if any
  virtual std::pair<bool, std::chrono::nanoseconds> peek_next_receive_time() = 0;

  //! Request the next message to be dispatched
  virtual void dispatch(Dispatcher &) = 0;
};

}  // namespace roq
