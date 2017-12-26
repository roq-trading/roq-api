/* Copyright (c) 2017, Hans Erik Thrane */

#pragma once

#include <quinclas/server.h>

namespace examples {
namespace gateway {

class Gateway : public quinclas::common::Gateway2 {
 public:
  Gateway(quinclas::common::Gateway2::Dispatcher& dispatcher, const int latency);

 protected:
  void start() override;
  void on(const quinclas::common::CreateOrderRequest&) override;
  void on(const quinclas::common::ModifyOrderRequest&) override;
  void on(const quinclas::common::CancelOrderRequest&) override;

 private:
  quinclas::common::Gateway2::Dispatcher& _dispatcher;
  int _latency;
};

}  // namespace gateway
}  // namespace examples
