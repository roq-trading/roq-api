/* Copyright (c) 2017, Hans Erik Thrane */

#pragma once

namespace examples {
namespace gateway_simulator {

class Strategy : public examples::gateway_simulator::Client {
 public:
  Strategy(examples::gateway_simulator::Client::Writer& writer, const int latency)
      : _writer(writer), _latency(latency) {}

 private:
  void on_read() override {
  }

 private:
  examples::gateway_simulator::Client::Writer& _writer;
  int _latency;
};

}  // namespace gateway_simulator
}  // namespace examples
