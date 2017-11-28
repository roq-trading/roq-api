/* Copyright (c) 2017, Hans Erik Thrane */

#include "execution_engine/strategy.h"

namespace quinclas {
namespace execution_engine {

Strategy::Strategy(common::Strategy::Dispatcher& dispatcher) :
    _dispatcher(dispatcher) {
}

void Strategy::on(const common::IdleEvent&) {
}

void Strategy::on(const common::GatewayStatusEvent&) {
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
