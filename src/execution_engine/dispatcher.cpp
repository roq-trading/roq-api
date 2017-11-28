/* Copyright (c) 2017, Hans Erik Thrane */

#include "execution_engine/dispatcher.h"
#include <glog/logging.h>
#include <quinclas/tradingapi.h>
#include <quinclas/codec.h>

namespace quinclas {
namespace execution_engine {

Dispatcher::Dispatcher(event::BufferEvent& buffer_event) :
    _buffer_event(buffer_event) {}

void Dispatcher::send(const common::CreateOrderRequest& create_order_request) {
    convert(_flat_buffer_builder, create_order_request);
    send(1);
}

void Dispatcher::send(const common::ModifyOrderRequest& modify_order_request) {
    convert(_flat_buffer_builder, modify_order_request);
    send(2);
}

void Dispatcher::send(const common::CancelOrderRequest& cancel_order_request) {
    convert(_flat_buffer_builder, cancel_order_request);
    send(3);
}

void Dispatcher::send(uint8_t msg_id) {
    size_t size = _flat_buffer_builder.GetSize();
    uint8_t *buffer = _flat_buffer_builder.GetBufferPointer();
    // TODO(thraneh): envelope
    // TODO(thraneh): send
    _flat_buffer_builder.Clear();
}

}  // namespace execution_engine
}  // namespace quinclas
