/* Copyright (c) 2017, Hans Erik Thrane */

#pragma once

#include <sys/types.h>
#include <sys/un.h>
#include <quinclas/codec.h>
#include <quinclas/io/libevent.h>

#include <glog/logging.h>
#include <string>

namespace quinclas {
namespace server {

class RequestDispatcher : public common::Strategy::Dispatcher {
 public:
  explicit RequestDispatcher(io::libevent::BufferEvent& buffer_event) :
      _buffer_event(buffer_event) {}

 private:
  void send(const common::CreateOrderRequest& create_order_request) override {
    _flat_buffer_builder.Clear();
    convert(_flat_buffer_builder, create_order_request);
    send();
  }
  void send(const common::ModifyOrderRequest& modify_order_request) override {
    _flat_buffer_builder.Clear();
    convert(_flat_buffer_builder, modify_order_request);
    send();
  }
  void send(const common::CancelOrderRequest& cancel_order_request) override {
    _flat_buffer_builder.Clear();
    convert(_flat_buffer_builder, cancel_order_request);
    send();
  }
  void send() {
    const auto payload = _flat_buffer_builder.GetBufferPointer();
    const auto length_payload = _flat_buffer_builder.GetSize();
    common::Envelope::encode(_envelope, length_payload);
    _buffer.add(_envelope, sizeof(_envelope));
    _buffer.add(payload, length_payload);
    _buffer_event.write(_buffer);
  }
  io::libevent::Buffer _buffer;
  io::libevent::BufferEvent& _buffer_event;
  flatbuffers::FlatBufferBuilder _flat_buffer_builder;
  uint8_t _envelope[common::Envelope::LENGTH];
};

}  // namespace server
}  // namespace quinclas
