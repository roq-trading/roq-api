/* Copyright (c) 2017, Hans Erik Thrane */

#include "execution_engine/request_dispatcher.h"
#include <glog/logging.h>
#include <quinclas/codec.h>

namespace quinclas {
namespace execution_engine {

RequestDispatcher::RequestDispatcher(event::BufferEvent& buffer_event) :
    _buffer_event(buffer_event) {}

void RequestDispatcher::send(const common::CreateOrderRequest& create_order_request) {
    _flat_buffer_builder.Clear();
    convert(_flat_buffer_builder, create_order_request);
    send();
}

void RequestDispatcher::send(const common::ModifyOrderRequest& modify_order_request) {
    _flat_buffer_builder.Clear();
    convert(_flat_buffer_builder, modify_order_request);
    send();
}

void RequestDispatcher::send(const common::CancelOrderRequest& cancel_order_request) {
    _flat_buffer_builder.Clear();
    convert(_flat_buffer_builder, cancel_order_request);
    send();
}

void RequestDispatcher::send() {
    const auto payload = _flat_buffer_builder.GetBufferPointer();
    const auto length_payload = _flat_buffer_builder.GetSize();
    common::Envelope::encode(_envelope, length_payload);
    _buffer.add(_envelope, sizeof(_envelope));
    _buffer.add(payload, length_payload);
    _buffer_event.write(_buffer);
}

}  // namespace execution_engine
}  // namespace quinclas
