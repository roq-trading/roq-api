/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <utility>

#include "roq/api.hpp"

namespace roq {

//! Generator interface used to manage a sequence of messages
struct ROQ_PUBLIC Generator {
  //! Dispatch interface used to handle update events
  struct ROQ_PUBLIC Dispatcher {
    virtual ~Dispatcher() {}

    // config
    virtual void operator()(Event<GatewaySettings> const &) = 0;

    // stream
    virtual void operator()(Event<StreamStatus> const &) = 0;
    virtual void operator()(Event<ExternalLatency> const &) = 0;

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
