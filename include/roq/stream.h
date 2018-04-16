/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <ostream>
#include <vector>

namespace roq {

struct Number {
  explicit Number(double value) : _value(value) {}
  const double _value;
};

template <typename T>
struct Vector {
  explicit Vector(const std::vector<T>& value) : _value(value) {}
  const std::vector<T>& _value;
};

std::ostream& operator<<(std::ostream& stream, time_point_t value);
std::ostream& operator<<(std::ostream& stream, const Number value);

template <typename T>
std::ostream& operator<<(std::ostream& stream, const Vector<T> value);

std::ostream& operator<<(std::ostream& stream, const ConnectionStatus value);
std::ostream& operator<<(std::ostream& stream, const GatewayState value);
std::ostream& operator<<(std::ostream& stream, const Side value);
std::ostream& operator<<(std::ostream& stream, const OrderType value);
std::ostream& operator<<(std::ostream& stream, const TimeInForce value);
std::ostream& operator<<(std::ostream& stream, const TradingStatus value);
std::ostream& operator<<(std::ostream& stream, const OrderStatus value);
std::ostream& operator<<(std::ostream& stream, const Layer& value);

std::ostream& operator<<(std::ostream& stream, const Handshake& value);
std::ostream& operator<<(std::ostream& stream, const HandshakeAck& value);
std::ostream& operator<<(std::ostream& stream, const Heartbeat& value);
std::ostream& operator<<(std::ostream& stream, const HeartbeatAck& value);
std::ostream& operator<<(std::ostream& stream, const DownloadBegin& value);
std::ostream& operator<<(std::ostream& stream, const DownloadEnd& value);
std::ostream& operator<<(std::ostream& stream, const GatewayStatus& value);
std::ostream& operator<<(std::ostream& stream, const MarketByPrice& value);
std::ostream& operator<<(std::ostream& stream, const TradeSummary& value);
std::ostream& operator<<(std::ostream& stream, const ReferenceData& value);
std::ostream& operator<<(std::ostream& stream, const MarketStatus& value);
std::ostream& operator<<(std::ostream& stream, const PositionUpdate& value);
std::ostream& operator<<(std::ostream& stream, const OrderUpdate& value);
std::ostream& operator<<(std::ostream& stream, const TradeUpdate& value);
std::ostream& operator<<(std::ostream& stream, const CreateOrder& value);
std::ostream& operator<<(std::ostream& stream, const ModifyOrder& value);
std::ostream& operator<<(std::ostream& stream, const CancelOrder& value);
std::ostream& operator<<(std::ostream& stream, const CreateOrderAck& value);
std::ostream& operator<<(std::ostream& stream, const ModifyOrderAck& value);
std::ostream& operator<<(std::ostream& stream, const CancelOrderAck& value);

std::ostream& operator<<(std::ostream& stream, const MessageInfo& value);

std::ostream& operator<<(std::ostream& stream, const ConnectionStatusEvent& value);

std::ostream& operator<<(std::ostream& stream, const BatchBeginEvent& value);
std::ostream& operator<<(std::ostream& stream, const BatchEndEvent& value);
std::ostream& operator<<(std::ostream& stream, const HandshakeEvent& value);
std::ostream& operator<<(std::ostream& stream, const HandshakeAckEvent& value);
std::ostream& operator<<(std::ostream& stream, const HeartbeatEvent& value);
std::ostream& operator<<(std::ostream& stream, const HeartbeatAckEvent& value);
std::ostream& operator<<(std::ostream& stream, const DownloadBeginEvent& value);
std::ostream& operator<<(std::ostream& stream, const DownloadEndEvent& value);
std::ostream& operator<<(std::ostream& stream, const GatewayStatusEvent& value);
std::ostream& operator<<(std::ostream& stream, const MarketByPriceEvent& value);
std::ostream& operator<<(std::ostream& stream, const TradeSummaryEvent& value);
std::ostream& operator<<(std::ostream& stream, const ReferenceDataEvent& value);
std::ostream& operator<<(std::ostream& stream, const MarketStatusEvent& value);
std::ostream& operator<<(std::ostream& stream, const PositionUpdateEvent& value);
std::ostream& operator<<(std::ostream& stream, const OrderUpdateEvent& value);
std::ostream& operator<<(std::ostream& stream, const TradeUpdateEvent& value);
std::ostream& operator<<(std::ostream& stream, const CreateOrderEvent& value);
std::ostream& operator<<(std::ostream& stream, const ModifyOrderEvent& value);
std::ostream& operator<<(std::ostream& stream, const CancelOrderEvent& value);
std::ostream& operator<<(std::ostream& stream, const CreateOrderAckEvent& value);
std::ostream& operator<<(std::ostream& stream, const ModifyOrderAckEvent& value);
std::ostream& operator<<(std::ostream& stream, const CancelOrderAckEvent& value);

}  // namespace roq
