/* Copyright (c) 2017, Hans Erik Thrane */

#pragma once

#include <quinclas/server.h>

namespace examples {
namespace gateway {

class Strategy : public quinclas::server::Client {
 public:
  Strategy(quinclas::server::Client::Writer& writer, const int latency)
      : _writer(writer), _latency(latency) {}

 private:
  void on_read() override {
  }

 private:
  quinclas::server::Client::Writer& _writer;
  int _latency;
};

}  // namespace gateway
}  // namespace examples
