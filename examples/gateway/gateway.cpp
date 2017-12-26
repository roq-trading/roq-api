/* Copyright (c) 2017, Hans Erik Thrane */

#include "gateway/gateway.h"

using namespace quinclas::common;  // NOLINT

namespace examples {
namespace gateway {

Gateway::Gateway(quinclas::common::Gateway::Handler& dispatcher, const int latency)
    : _dispatcher(dispatcher), _latency(latency) {}

void Gateway::start() {
  // if you work with a 3rd party API, this is a good place to spawn a thread
}

void Gateway::send(const quinclas::common::CreateOrderRequest& request) {
  // the client has requested an order to be created
}
void Gateway::send(const quinclas::common::ModifyOrderRequest& request) {
  // the client has requested an order to be modified
}
void Gateway::send(const quinclas::common::CancelOrderRequest& request) {
  // the client has requested an order to be cancelled
}

}  // namespace gateway
}  // namespace examples
