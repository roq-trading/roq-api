/* Copyright (c) 2017, Hans Erik Thrane */

#pragma once

#include <quinclas/codec.h>
#include "execution_engine/event.h"

namespace quinclas {
namespace execution_engine {

class EventDispatcher : public common::EventDispatcher {
 public:
    EventDispatcher(common::Strategy& strategy, event::Base& base, event::BufferEvent& buffer_event);
 private:
    static void on_error(struct bufferevent *bev, short what, void *arg);
    static void on_read(struct bufferevent *bev, void *arg);
    void on_read();
 private:
    event::Base& _base;
    event::BufferEvent& _buffer_event;
    event::Buffer _buffer;
};

}  // namespace execution_engine
}  // namespace quinclas
