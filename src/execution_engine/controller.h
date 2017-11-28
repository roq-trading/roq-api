/* Copyright (c) 2017, Hans Erik Thrane */

#pragma once

#include <quinclas/tradingapi.h>
#include <flatbuffers/flatbuffers.h>

#include "execution_engine/event.h"

namespace quinclas {
namespace execution_engine {

class Controller {
 public:
    Controller(event::Base& base, event::BufferEvent& buffer_event, common::Strategy& strategy);
 private:
    static void on_error(struct bufferevent *bev, short what, void *arg);
    static void on_read(struct bufferevent *bev, void *arg);
    void on_read();
 private:
    event::Base& _base;
    event::BufferEvent& _buffer_event;
    common::Strategy& _strategy;
    event::Buffer _buffer;
    flatbuffers::FlatBufferBuilder _flat_buffer_builder;
};

}  // namespace execution_engine
}  // namespace quinclas
