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

  virtual void operator()(const DownloadBeginEvent&) = 0;
  virtual void operator()(const DownloadEndEvent&) = 0;
  virtual void operator()(const MarketDataStatusEvent&) = 0;
  virtual void operator()(const OrderManagerStatusEvent&) = 0;
  virtual void operator()(const ReferenceDataEvent&) = 0;
  virtual void operator()(const MarketStatusEvent&) = 0;
  virtual void operator()(const TopOfBookEvent&) = 0;
  virtual void operator()(const MarketByPriceEvent&) = 0;
  virtual void operator()(const MarketByOrderEvent&) = 0;
  virtual void operator()(const TradeSummaryEvent&) = 0;
  virtual void operator()(const SessionStatisticsEvent&) = 0;
  virtual void operator()(const DailyStatisticsEvent&) = 0;
  virtual void operator()(const CreateOrderEvent&) = 0;
  virtual void operator()(const ModifyOrderEvent&) = 0;
  virtual void operator()(const CancelOrderEvent&) = 0;
  virtual void operator()(const OrderAckEvent&) = 0;
  virtual void operator()(const OrderUpdateEvent&) = 0;
  virtual void operator()(const TradeUpdateEvent&) = 0;
  virtual void operator()(const PositionUpdateEvent&) = 0;
  virtual void operator()(const FundsUpdateEvent&) = 0;

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
