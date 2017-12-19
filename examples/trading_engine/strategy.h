/* Copyright (c) 2017, Hans Erik Thrane */

#pragma once

#include <quinclas/tradingapi.h>

namespace examples {
namespace trading_engine {

class Strategy : public quinclas::common::Strategy {
 public:
  Strategy(quinclas::common::Strategy::Dispatcher& dispatcher, const uint32_t ticks_to_trade);

 protected:
  void on(const quinclas::common::TimerEvent&) override;
  void on(const quinclas::common::IdleEvent&) override;
  void on(const quinclas::common::GatewayStatusEvent&) override;
  void on(const quinclas::common::ReferenceDataEvent&) override;
  void on(const quinclas::common::MarketStatusEvent&) override;
  void on(const quinclas::common::MarketByPriceEvent&) override;
  void on(const quinclas::common::SessionStatisticsEvent&) override;
  void on(const quinclas::common::DailyStatisticsEvent&) override;
  void on(const quinclas::common::CreateOrderAckEvent&) override;
  void on(const quinclas::common::ModifyOrderAckEvent&) override;
  void on(const quinclas::common::CancelOrderAckEvent&) override;
  void on(const quinclas::common::OrderUpdateEvent&) override;
  void on(const quinclas::common::TradeUpdateEvent&) override;

 private:
  quinclas::common::Strategy::Dispatcher& _dispatcher;
  const uint32_t _ticks_to_trade;
};

}  // namespace trading_engine
}  // namespace examples
