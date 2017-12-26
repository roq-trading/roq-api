/* Copyright (c) 2017, Hans Erik Thrane */

#pragma once

#include <quinclas/server.h>

namespace examples {
namespace gateway {

class Gateway : public quinclas::common::Gateway {
 public:
  Gateway(quinclas::common::Gateway::Handler& dispatcher, const int latency);

 protected:
  void start() override;
  void send(const quinclas::common::CreateOrderRequest&) override;
  void send(const quinclas::common::ModifyOrderRequest&) override;
  void send(const quinclas::common::CancelOrderRequest&) override;

 private:
  quinclas::common::Gateway::Handler& _dispatcher;
  int _latency;
};

}  // namespace gateway
}  // namespace examples
