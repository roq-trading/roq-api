/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "gateway/gateway.h"

using namespace quinclas::common;  // NOLINT

namespace examples {
namespace gateway {

Gateway::Gateway(quinclas::common::Gateway::Dispatcher& dispatcher, const int latency)
    : _dispatcher(dispatcher), _latency(latency) {
  // create objects, but don't spawn threads just yet
}

void Gateway::start() {
  // good place to spawn a thread when working with 3rd party APIs
}

void Gateway::get_metrics(std::ostream& stream, const char *format) {
  // option to log gateway internal statistics
}

void Gateway::on(const quinclas::common::CreateOrderRequest& request) {
  // a client has requested an order to be created
}
void Gateway::on(const quinclas::common::ModifyOrderRequest& request) {
  // a client has requested an order to be modified
}
void Gateway::on(const quinclas::common::CancelOrderRequest& request) {
  // a client has requested an order to be cancelled
}

}  // namespace gateway
}  // namespace examples
