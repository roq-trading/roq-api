/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <ostream>
#include <utility>

#include "roq/api.h"
#include "roq/traits.h"

namespace roq {

// enums

ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const ConnectionStatus);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const GatewayStatus);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const Side);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const UpdateAction);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const OrderType);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const TimeInForce);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const PositionEffect);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const TradingStatus);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const OrderStatus);

// helpers

ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const MBPUpdate&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const MessageInfo&);

// messages

ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const Subscribe&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const DownloadBegin&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const DownloadEnd&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const MarketDataStatus&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const OrderManagerStatus&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const SessionStatistics&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const DailyStatistics&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const MarketByPrice&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const TradeSummary&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const ReferenceData&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const MarketStatus&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const PositionUpdate&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const OrderUpdate&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const TradeUpdate&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const CreateOrder&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const ModifyOrder&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const CancelOrder&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const CreateOrderAck&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const ModifyOrderAck&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const CancelOrderAck&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const CustomMessage&);

// events

ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const StartEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const StopEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const TimerEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const ConnectionStatusEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const BatchBeginEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const BatchEndEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const SubscribeEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const DownloadBeginEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const DownloadEndEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const MarketDataStatusEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const OrderManagerStatusEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const SessionStatisticsEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const DailyStatisticsEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const MarketByPriceEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const TradeSummaryEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const ReferenceDataEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const MarketStatusEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const PositionUpdateEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const OrderUpdateEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const TradeUpdateEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const CreateOrderEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const ModifyOrderEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const CancelOrderEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const CreateOrderAckEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const ModifyOrderAckEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const CancelOrderAckEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const CustomMessageEvent&);

// utilities

ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const Layer&);

// misc

ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const Account&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const User&);

// helper:

namespace detail {
template <typename T> struct wrapper {
  explicit wrapper(const T& v) noexcept : v(v) {
  }
  const T& v;
};
template <typename T>
inline std::ostream& operator<<(
    std::ostream& stream,
    const wrapper<T>& value) {
  if constexpr (is_pair<T>::value) {
    return stream << wrapper(value.v.first) << "=" << wrapper(value.v.second);
  } else {
    if constexpr (is_string<T>::value) {
      return stream << "\"" << value.v << "\"";
    } else {
      if constexpr (is_iterable<T>::value) {
        bool first = true;
        stream << "{";
        for (auto iter = value.v.begin(); iter != value.v.end(); ++iter) {
          if (first) {
            first = false;
          } else {
            stream << ", ";
          }
          stream << wrapper(*iter);
        }
        return stream << "}";
      } else {
        return stream << value.v;
      }
    }
  }
}
}  // namespace detail

// join
template <typename T>
struct join {
  explicit join(const T& v) noexcept : v(v) {
  }
  const T& v;
};
template <typename T>
std::ostream& operator<<(
    std::ostream& stream,
    const join<T>& value) {
  return stream << detail::wrapper(value.v);
}

}  // namespace roq
