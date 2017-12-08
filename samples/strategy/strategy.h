/* Copyright (c) 2017, Hans Erik Thrane */

#pragma once

#include <quinclas/tradingapi.h>
#include <quinclas/io/libevent.h>  // FIXME(thraneha): NOT HERE
#include <limits>

namespace quinclas {
namespace strategy {

class Strategy : public io::libevent::TimerEvent::Handler, public common::Strategy {
 public:
  explicit Strategy(common::Strategy::Dispatcher& dispatcher);

 protected:
  void on_timer() override;
  void on(const common::IdleEvent&) override;
  void on(const common::GatewayStatusEvent&) override;
  void on(const common::ReferenceDataEvent&) override;
  void on(const common::MarketStatusEvent&) override;
  void on(const common::MarketByPriceEvent&) override;
  void on(const common::SessionStatisticsEvent&) override;
  void on(const common::DailyStatisticsEvent&) override;
  void on(const common::CreateOrderAckEvent&) override;
  void on(const common::ModifyOrderAckEvent&) override;
  void on(const common::CancelOrderAckEvent&) override;
  void on(const common::OrderUpdateEvent&) override;
  void on(const common::TradeUpdateEvent&) override;

 private:
  common::Strategy::Dispatcher& _dispatcher;
};

}  // namespace execution_engine
}  // namespace quinclas
