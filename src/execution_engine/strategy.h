/* Copyright (c) 2017, Hans Erik Thrane */

#include <quinclas/tradingapi.h>

namespace quinclas {
namespace execution_engine {

class Strategy : public common::Strategy {
 public:
    Strategy(common::Strategy::Dispatcher& dispatcher);
 protected:
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
