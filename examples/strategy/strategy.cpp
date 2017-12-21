/* Copyright (c) 2017, Hans Erik Thrane */

#include "strategy/strategy.h"

#include <limits>

using namespace quinclas::common;  // NOLINT

namespace examples {
namespace strategy {

// some hard-coded config (for now)

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
  // you can only use timer events to guarantee a certain point in time has been reached
  // for example, you may decide a submitted order has timed out
  // however, do not rely on the accuracy or frequency of the timer!
}

void Strategy::on(const IdleEvent&) {
  // you can use idle events to process updates in batch
  // for example, the inbound queue may contain multiple updates to the same market depth
  // your implementation strategy could be such that you set a dirty flag and otherwise only
  // make a copy each time you receive the market depth update event (perhaps also do light-
  // weight processing)
  // you can then use this idle event to detect market depth has been changed and then begin
  // heavy-weight processing knowing that you can't possibly fall behind
  // the idle event is therefore your tool to implement flow-control (e.g. conflation as just
  // described in the example)
}

void Strategy::on(const GatewayStatusEvent& event) {
  // every change to gateway connection and/or login status will be communicated here
  // changes include
  // - gateway unable to connect
  // - gateway disconnect
  // - gateway user being logged in
  // - gateway user being remotely logged out
  // - this application being disconnected from the gateway
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
  // reference data update per instrument, e.g. tick-size
}

void Strategy::on(const MarketStatusEvent&) {
  // market status update, e.g. status is "open for trading"
}

void Strategy::on(const MarketByPriceEvent&) {
  // market depth update by instrument
}

void Strategy::on(const SessionStatisticsEvent&) {
  // session statistics per instrument, e.g. open/high/low prices
}

void Strategy::on(const DailyStatisticsEvent&) {
  // daily statistics per instrument, e.g. open interest
}

void Strategy::on(const CreateOrderAckEvent&) {
  // response to your create order request
  // note! a response is not guaranteed
}

void Strategy::on(const ModifyOrderAckEvent&) {
  // response to your modify order request
  // note! a response is not guaranteed
}

void Strategy::on(const CancelOrderAckEvent&) {
  // response to your cancel order request
  // note! a response is not guaranteed
}

void Strategy::on(const OrderUpdateEvent&) {
  // order update
}

void Strategy::on(const TradeUpdateEvent&) {
  // trade update
}

}  // namespace strategy
}  // namespace examples
