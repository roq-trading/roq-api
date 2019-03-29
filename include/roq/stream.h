/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <ostream>

#include "roq/api.h"

// enums

ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::ConnectionStatus);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::GatewayStatus);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::Side);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::UpdateAction);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::OrderType);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::TimeInForce);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::PositionEffect);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::TradingStatus);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::OrderStatus);

// helpers

ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::MBPUpdate&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::MessageInfo&);

// messages

ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::Subscribe&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::DownloadBegin&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::DownloadEnd&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::MarketDataStatus&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::OrderManagerStatus&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::SessionStatistics&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::DailyStatistics&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::MarketByPrice&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::TradeSummary&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::ReferenceData&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::MarketStatus&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::PositionUpdate&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::OrderUpdate&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::TradeUpdate&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::CreateOrder&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::ModifyOrder&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::CancelOrder&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::CreateOrderAck&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::ModifyOrderAck&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::CancelOrderAck&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::CustomMessage&);

// events

ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::StartEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::StopEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::TimerEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::ConnectionStatusEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::BatchBeginEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::BatchEndEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::SubscribeEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::DownloadBeginEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::DownloadEndEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::MarketDataStatusEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::OrderManagerStatusEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::SessionStatisticsEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::DailyStatisticsEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::MarketByPriceEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::TradeSummaryEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::ReferenceDataEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::MarketStatusEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::PositionUpdateEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::OrderUpdateEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::TradeUpdateEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::CreateOrderEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::ModifyOrderEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::CancelOrderEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::CreateOrderAckEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::ModifyOrderAckEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::CancelOrderAckEvent&);
ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::CustomMessageEvent&);

// utilities

ROQ_PUBLIC std::ostream& operator<<(std::ostream&, const roq::Layer&);
