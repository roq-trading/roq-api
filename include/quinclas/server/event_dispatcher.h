/* Copyright (c) 2017, Hans Erik Thrane */

#pragma once

#include <quinclas/codec.h>
#include <glog/logging.h>
#include <quinclas/io/libevent.h>

namespace quinclas {
namespace server {

// TODO(thraneh): reconnect instead of shutdown

class EventDispatcher : public common::EventDispatcher {
 public:
  EventDispatcher(common::Strategy& strategy, io::libevent::Base& base, io::libevent::BufferEvent& buffer_event) :
    common::EventDispatcher(strategy),
    _base(base),
    _buffer_event(buffer_event) {
    _buffer_event.setcb(on_read, nullptr, on_error, this);
    _buffer_event.enable(EV_READ);
  }

 private:
  io::libevent::Base& _base;
  io::libevent::BufferEvent& _buffer_event;
  io::libevent::Buffer _buffer;
  static void on_error(struct bufferevent *bev, short what, void *arg) {  // NOLINT short
    auto& self = *reinterpret_cast<EventDispatcher*>(arg);
    if (what & BEV_EVENT_EOF)
      LOG(INFO) << "Client disconnected";
    else
      LOG(WARNING) << "Socket error";
    self._base.loopexit();
  }
  static void on_read(struct bufferevent *bev, void *arg) {
    reinterpret_cast<EventDispatcher*>(arg)->on_read();
  }
  void on_read() {
    try {
      _buffer_event.read(_buffer);
      while (true) {
        const auto envelope = _buffer.pullup(common::Envelope::LENGTH);
        if (envelope == nullptr)
          break;
        const auto length_payload = common::Envelope::decode(envelope);
        const auto bytes = common::Envelope::LENGTH + length_payload;
        const auto frame = _buffer.pullup(bytes);
        if (frame == nullptr)
          break;
        const auto payload = frame + common::Envelope::LENGTH;
        dispatch_event(payload, length_payload);
        _buffer.drain(bytes);
      }
      return;
    } catch (std::exception& e) {
      LOG(WARNING) << "Exception: " << e.what();
    }
    catch (...) {
      LOG(ERROR) << "Exception: <unknown>";
    }
    _base.loopexit();
  }
};

}  // namespace server
}  // namespace quinclas
