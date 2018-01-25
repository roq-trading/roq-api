/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <quinclas/server.h>

namespace examples {
namespace gateway {

class Gateway : public quinclas::common::Gateway {
 public:
  Gateway(quinclas::common::Gateway::Dispatcher& dispatcher, const int latency);

 protected:
  void start() override;
  void stop() override;
  void get_metrics(std::ostream& stream, const char *format) override;
  void on(const quinclas::common::CreateOrderRequest&) override;
  void on(const quinclas::common::ModifyOrderRequest&) override;
  void on(const quinclas::common::CancelOrderRequest&) override;

 private:
  // dispatch interface (used to broadcast update events)
  quinclas::common::Gateway::Dispatcher& _dispatcher;
  // gateway specific configuration
  int _latency;
};

}  // namespace gateway
}  // namespace examples
