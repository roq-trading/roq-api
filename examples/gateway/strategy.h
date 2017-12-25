/* Copyright (c) 2017, Hans Erik Thrane */

#pragma once

#include <quinclas/server.h>

namespace examples {
namespace gateway {

class Strategy : public quinclas::server::Connection {
 public:
  Strategy(quinclas::server::Connection::Dispatcher& dispatcher, const int latency)
      : _dispatcher(dispatcher), _latency(latency) {}

 private:
  void send(const quinclas::common::CreateOrderRequest& request) override {
    // TODO(thraneh): implement
  }
  void send(const quinclas::common::ModifyOrderRequest& request) override {
    // TODO(thraneh): implement
  }
  void send(const quinclas::common::CancelOrderRequest& request) override {
    // TODO(thraneh): implement
  }

 private:
  quinclas::server::Client::Dispatcher& _dispatcher;
  int _latency;
};

}  // namespace gateway
}  // namespace examples
