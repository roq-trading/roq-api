/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "reference/strategy.h"
#include <glog/logging.h>

using namespace quinclas::common;  // NOLINT

namespace examples {
namespace reference {

namespace {
static void log(const Strategy::Config& config) {
  LOG(INFO) << "Config = {"
      "time_zone=\"" << config.time_zone.name() << "\", "
      "instrument=\"" << config.instrument << "\", "
      "exchange=\"" << config.exchange << "\", "
      "model_params=(" <<
      config.model_params.first << ", " <<
      config.model_params.second <<
      ")"
      "}";
}
}  // namespace

Strategy::Strategy(quinclas::common::Strategy::Dispatcher& dispatcher,
                   Config&& config)
    : _dispatcher(dispatcher), _config(std::move(config)) {
  log(_config);
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
  if (_config.instrument.compare(event.market_by_price.instrument) == 0)
    LOG(INFO) << event;
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
