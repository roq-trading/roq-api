/* Copyright (c) 2017, Hans Erik Thrane */

#include "execution_engine/strategy.h"

namespace quinclas {
namespace execution_engine {

const char    *GATEWAY        = "FEMAS1";
const char    *EXCHANGE       = "CFFEX";
const char    *INSTRUMENT     = "IF1712";
const uint32_t QUANTITY       =  1;        // CAREFUL
const double   LIMIT_PRICE    =  0.01;     // CAREFUL
const char    *ORDER_TEMPLATE = "ioc_open";

Strategy::Strategy(common::Strategy::Dispatcher& dispatcher) :
  _dispatcher(dispatcher) {
}

void Strategy::on_timer() {
}

void Strategy::on(const common::IdleEvent&) {
}

void Strategy::on(const common::GatewayStatusEvent& event) {
  if (event.gateway_status.market_data_login_status == common::LoginStatus::On &&
      event.gateway_status.order_management_login_status == common::LoginStatus::On) {
    const common::CreateOrderRequest create_order_request = {
      .request_info = {
        .destination = GATEWAY,
      },
      .create_order = {
        .exchange            = EXCHANGE,
        .order_template_name = ORDER_TEMPLATE,
        .instrument          = INSTRUMENT,
        .direction           = common::TradeDirection::Buy,
        .quantity            = QUANTITY,
        .limit_price         = LIMIT_PRICE,
        .stop_price          = std::numeric_limits<double>::quiet_NaN(),
        .opaque              = 1,
      }
    };
    _dispatcher.send(create_order_request);
  }
}

void Strategy::on(const common::ReferenceDataEvent&) {
}

void Strategy::on(const common::MarketStatusEvent&) {
}

void Strategy::on(const common::MarketByPriceEvent&) {
}

void Strategy::on(const common::SessionStatisticsEvent&) {
}

void Strategy::on(const common::DailyStatisticsEvent&) {
}

void Strategy::on(const common::CreateOrderAckEvent&) {
}

void Strategy::on(const common::ModifyOrderAckEvent&) {
}

void Strategy::on(const common::CancelOrderAckEvent&) {
}

void Strategy::on(const common::OrderUpdateEvent&) {
}

void Strategy::on(const common::TradeUpdateEvent&) {
}

}  // namespace execution_engine
}  // namespace quinclas
