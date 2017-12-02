/* Copyright (c) 2017, Hans Erik Thrane */

#include "execution_engine/controller.h"
#include <glog/logging.h>
#include <quinclas/codec.h>

#define LENGTH_ENVELOPE 8

namespace quinclas {
namespace execution_engine {

Controller::Controller(event::Base& base, event::BufferEvent& buffer_event, common::Strategy& strategy) :
    _base(base),
    _buffer_event(buffer_event),
    _strategy(strategy) {
    _buffer_event.setcb(on_read, nullptr, on_error, this);
    _buffer_event.enable(EV_READ);
}

void Controller::on_error(struct bufferevent *bev, short what, void *arg) {
    auto& self = *reinterpret_cast<Controller*>(arg);
    if (what & BEV_EVENT_EOF)
        LOG(INFO) << "Client disconnected";
    else
        LOG(WARNING) << "Socket error";
    self._base.loopexit();
}

void Controller::on_read(struct bufferevent *bev, void *arg) {
    reinterpret_cast<Controller*>(arg)->on_read();
}

void Controller::on_read() {
    _buffer_event.read(_buffer);
    unsigned char *data = _buffer.pullup(common::Envelope::LENGTH);
    if (data) {
        common::Envelope envelope(data);
        auto length_payload = envelope.length();
        data = _buffer.pullup(LENGTH_ENVELOPE + length_payload);
        if (data) {
            data += LENGTH_ENVELOPE;
            // FIXME(thraneha): remember envelope.type disappears !!!
            try {
                common::dispatch(_strategy, data, length_payload);
            }
            catch (...) {
            }
            _buffer.drain(LENGTH_ENVELOPE + length_payload);
        }
    }
}

}  // namespace execution_engine
}  // namespace quinclas
