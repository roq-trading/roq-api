/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include "roq/api.h"

namespace roq {

// event_value

template <typename E>
auto event_value(const E&);

template <>
inline auto event_value(const ConnectionStatusEvent& event) {
  return event.connection_status;
}

template <>
inline auto event_value(const DownloadBeginEvent& event) {
  return event.download_begin;
}

template <>
inline auto event_value(const DownloadEndEvent& event) {
  return event.download_end;
}

template <>
inline auto event_value(const MarketDataStatusEvent& event) {
  return event.market_data_status;
}

template <>
inline auto event_value(const OrderManagerStatusEvent& event) {
  return event.order_manager_status;
}

template <>
inline auto event_value(const SessionStatisticsEvent& event) {
  return event.session_statistics;
}

template <>
inline auto event_value(const DailyStatisticsEvent& event) {
  return event.daily_statistics;
}

template <>
inline auto event_value(const MarketByPriceEvent& event) {
  return event.market_by_price;
}

template <>
inline auto event_value(const TradeSummaryEvent& event) {
  return event.trade_summary;
}

template <>
inline auto event_value(const ReferenceDataEvent& event) {
  return event.reference_data;
}

template <>
inline auto event_value(const MarketStatusEvent& event) {
  return event.market_status;
}

template <>
inline auto event_value(const PositionUpdateEvent& event) {
  return event.position_update;
}

template <>
inline auto event_value(const OrderUpdateEvent& event) {
  return event.order_update;
}

template <>
inline auto event_value(const TradeUpdateEvent& event) {
  return event.trade_update;
}

template <>
inline auto event_value(const RequestUpdateEvent& event) {
  return event.request_update;
}

template <>
inline auto event_value(const CreateOrderAckEvent& event) {
  return event.create_order_ack;
}

template <>
inline auto event_value(const ModifyOrderAckEvent& event) {
  return event.modify_order_ack;
}

template <>
inline auto event_value(const CancelOrderAckEvent& event) {
  return event.cancel_order_ack;
}

template <>
inline auto event_value(const CustomMessageEvent& event) {
  return event.custom_message;
}

// create_event

template <typename E>
auto create_event(const MessageInfo&, const E&);

template <>
inline auto create_event(
    const MessageInfo& message_info,
    const DownloadBeginEvent& event) {
  return decltype(event) {
    .message_info = message_info,
    .download_begin = event.download_begin
  };
}

template <>
inline auto create_event(
    const MessageInfo& message_info,
    const DownloadEndEvent& event) {
  return decltype(event) {
    .message_info = message_info,
    .download_end = event.download_end
  };
}

template <>
inline auto create_event(
    const MessageInfo& message_info,
    const MarketDataStatusEvent& event) {
  return decltype(event) {
    .message_info = message_info,
    .market_data_status = event.market_data_status
  };
}

template <>
inline auto create_event(
    const MessageInfo& message_info,
    const OrderManagerStatusEvent& event) {
  return decltype(event) {
    .message_info = message_info,
    .order_manager_status = event.order_manager_status
  };
}

template <>
inline auto create_event(
    const MessageInfo& message_info,
    const SessionStatisticsEvent& event) {
  return decltype(event) {
    .message_info = message_info,
    .session_statistics = event.session_statistics
  };
}

template <>
inline auto create_event(
    const MessageInfo& message_info,
    const DailyStatisticsEvent& event) {
  return decltype(event) {
    .message_info = message_info,
    .daily_statistics = event.daily_statistics
  };
}

template <>
inline auto create_event(
    const MessageInfo& message_info,
    const MarketByPriceEvent& event) {
  return decltype(event) {
    .message_info = message_info,
    .market_by_price = event.market_by_price
  };
}

template <>
inline auto create_event(
    const MessageInfo& message_info,
    const TradeSummaryEvent& event) {
  return decltype(event) {
    .message_info = message_info,
    .trade_summary = event.trade_summary
  };
}

template <>
inline auto create_event(
    const MessageInfo& message_info,
    const ReferenceDataEvent& event) {
  return decltype(event) {
    .message_info = message_info,
    .reference_data = event.reference_data
  };
}

template <>
inline auto create_event(
    const MessageInfo& message_info,
    const MarketStatusEvent& event) {
  return decltype(event) {
    .message_info = message_info,
    .market_status = event.market_status
  };
}

template <>
inline auto create_event(
    const MessageInfo& message_info,
    const PositionUpdateEvent& event) {
  return decltype(event) {
    .message_info = message_info,
    .position_update = event.position_update
  };
}

template <>
inline auto create_event(
    const MessageInfo& message_info,
    const OrderUpdateEvent& event) {
  return decltype(event) {
    .message_info = message_info,
    .order_update = event.order_update
  };
}

template <>
inline auto create_event(
    const MessageInfo& message_info,
    const TradeUpdateEvent& event) {
  return decltype(event) {
    .message_info = message_info,
    .trade_update = event.trade_update
  };
}

template <>
inline auto create_event(
    const MessageInfo& message_info,
    const CreateOrderAckEvent& event) {
  return decltype(event) {
    .message_info = message_info,
    .create_order_ack = event.create_order_ack
  };
}

template <>
inline auto create_event(
    const MessageInfo& message_info,
    const ModifyOrderAckEvent& event) {
  return decltype(event) {
    .message_info = message_info,
    .modify_order_ack = event.modify_order_ack
  };
}

template <>
inline auto create_event(
    const MessageInfo& message_info,
    const CancelOrderAckEvent& event) {
  return decltype(event) {
    .message_info = message_info,
    .cancel_order_ack = event.cancel_order_ack
  };
}

template <>
inline auto create_event(
    const MessageInfo& message_info,
    const CustomMessageEvent& event) {
  return decltype(event) {
    .message_info = message_info,
    .custom_message = event.custom_message
  };
}

}  // namespace roq
