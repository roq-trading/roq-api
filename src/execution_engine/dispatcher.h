/* Copyright (c) 2017, Hans Erik Thrane */

#pragma once

#include <sys/types.h>
#include <sys/un.h>
#include <quinclas/tradingapi.h>
#include <string>

#include "execution_engine/event.h"

namespace quinclas {
namespace execution_engine {

class Dispatcher : public common::Strategy::Dispatcher {
 public:
    explicit Dispatcher(event::BufferEvent& buffer_event);
 protected:
    void send(const common::CreateOrderRequest& create_order_request) override;
    void send(const common::ModifyOrderRequest& modify_order_request) override;
    void send(const common::CancelOrderRequest& cancel_order_request) override;
    void send(uint8_t msg_id);
 private:
    event::BufferEvent& _buffer_event;
    flatbuffers::FlatBufferBuilder _flat_buffer_builder;
};

}  // namespace execution_engine
}  // namespace quinclas
