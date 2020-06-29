/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include "roq/api.h"

#include "roq/client/data_frame.h"

namespace roq {
namespace client {

class Collector {
 public:
  virtual ~Collector() {}

  virtual void operator()(const Event<DownloadBegin>&) = 0;
  virtual void operator()(const Event<DownloadEnd>&) = 0;
  virtual void operator()(const Event<MarketDataStatus>&) = 0;
  virtual void operator()(const Event<OrderManagerStatus>&) = 0;
  virtual void operator()(const Event<ReferenceData>&) = 0;
  virtual void operator()(const Event<MarketStatus>&) = 0;
  virtual void operator()(const Event<TopOfBook>&) = 0;
  virtual void operator()(const Event<MarketByPriceUpdate>&) = 0;
  virtual void operator()(const Event<MarketByOrderUpdate>&) = 0;
  virtual void operator()(const Event<TradeSummary>&) = 0;
  virtual void operator()(const Event<StatisticsUpdate>&) = 0;
  virtual void operator()(const Event<CreateOrder>&) = 0;
  virtual void operator()(const Event<ModifyOrder>&) = 0;
  virtual void operator()(const Event<CancelOrder>&) = 0;
  virtual void operator()(const Event<OrderAck>&) = 0;
  virtual void operator()(const Event<OrderUpdate>&) = 0;
  virtual void operator()(const Event<TradeUpdate>&) = 0;
  virtual void operator()(const Event<PositionUpdate>&) = 0;
  virtual void operator()(const Event<FundsUpdate>&) = 0;

  virtual void extract(
      const std::string_view& type,
      const std::unordered_map<std::string, std::string>& selector,
      DataFrame::Handler& handler) const = 0;

  virtual void write(
      const std::string_view& directory,
      const std::string_view& format) const = 0;
};

}  // namespace client
}  // namespace roq
