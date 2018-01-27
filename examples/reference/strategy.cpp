/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/strategy.h"
#include <glog/logging.h>

using namespace quinclas::common;  // NOLINT

namespace examples {
namespace reference {

Strategy::Strategy(Strategy::Dispatcher& dispatcher, const Config& config)
    : _dispatcher(dispatcher),
      _config(config) {}

void Strategy::on(const TimerEvent& event) {
}

void Strategy::on(const IdleEvent&) {
}

void Strategy::on(const GatewayStatusEvent& event) {
}

void Strategy::on(const ReferenceDataEvent& event) {
}

void Strategy::on(const MarketStatusEvent& event) {
}

void Strategy::on(const MarketByPriceEvent& event) {
}

void Strategy::on(const SessionStatisticsEvent& event) {
}

void Strategy::on(const DailyStatisticsEvent& event) {
}

void Strategy::on(const CreateOrderAckEvent& event) {
}

void Strategy::on(const ModifyOrderAckEvent& event) {
}

void Strategy::on(const CancelOrderAckEvent& event) {
}

void Strategy::on(const OrderUpdateEvent& event) {
}

void Strategy::on(const TradeUpdateEvent& event) {
}

}  // namespace reference
}  // namespace examples
