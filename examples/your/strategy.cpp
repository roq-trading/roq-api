/* Copyright (c) 2017, Hans Erik Thrane */

#include "your/strategy.h"

#include <limits>

using namespace quinclas::common;  // NOLINT

namespace examples {
namespace your {

const char    *GATEWAY        = "FEMAS1";
const char    *EXCHANGE       = "CFFEX";
const char    *INSTRUMENT     = "IF1712";
const uint32_t QUANTITY       =  1;        // CAREFUL
const double   LIMIT_PRICE    =  0.01;     // CAREFUL
const char    *ORDER_TEMPLATE = "ioc_open";

Strategy::Strategy(Strategy::Dispatcher& dispatcher, const uint32_t ticks_to_trade)
    : _dispatcher(dispatcher),
      _ticks_to_trade(ticks_to_trade) {}

void Strategy::on(const TimerEvent&) {
}

void Strategy::on(const IdleEvent&) {
}

void Strategy::on(const GatewayStatusEvent& event) {
  if (event.gateway_status.market_data_login_status == LoginStatus::On &&
      event.gateway_status.order_management_login_status == LoginStatus::On) {
    const CreateOrderRequest create_order_request = {
      .request_info = {
        .destination = GATEWAY,
      },
      .create_order = {
        .exchange            = EXCHANGE,
        .order_template_name = ORDER_TEMPLATE,
        .instrument          = INSTRUMENT,
        .direction           = TradeDirection::Buy,
        .quantity            = QUANTITY,
        .limit_price         = LIMIT_PRICE,
        .stop_price          = std::numeric_limits<double>::quiet_NaN(),
        .opaque              = 1,
      }
    };
    _dispatcher.send(create_order_request);
  }
}

void Strategy::on(const ReferenceDataEvent&) {
}

void Strategy::on(const MarketStatusEvent&) {
}

void Strategy::on(const MarketByPriceEvent&) {
}

void Strategy::on(const SessionStatisticsEvent&) {
}

void Strategy::on(const DailyStatisticsEvent&) {
}

void Strategy::on(const CreateOrderAckEvent&) {
}

void Strategy::on(const ModifyOrderAckEvent&) {
}

void Strategy::on(const CancelOrderAckEvent&) {
}

void Strategy::on(const OrderUpdateEvent&) {
}

void Strategy::on(const TradeUpdateEvent&) {
}

}  // namespace your
}  // namespace examples
