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
            try {
                auto type = envelope.type();
                switch (type) {
                    case common::MessageType::GatewayStatusEvent:
                        _strategy.on(common::convert(flatbuffers::GetRoot<schema::GatewayStatusEvent>(data)));
                        break;
                    case common::MessageType::ReferenceDataEvent:
                        _strategy.on(common::convert(flatbuffers::GetRoot<schema::ReferenceDataEvent>(data)));
                        break;
                    case common::MessageType::MarketStatusEvent:
                        _strategy.on(common::convert(flatbuffers::GetRoot<schema::MarketStatusEvent>(data)));
                        break;
                    case common::MessageType::MarketByPriceEvent:
                        _strategy.on(common::convert(flatbuffers::GetRoot<schema::MarketByPriceEvent>(data)));
                        break;
                    case common::MessageType::SessionStatisticsEvent:
                        _strategy.on(common::convert(flatbuffers::GetRoot<schema::SessionStatisticsEvent>(data)));
                        break;
                    case common::MessageType::DailyStatisticsEvent:
                        _strategy.on(common::convert(flatbuffers::GetRoot<schema::DailyStatisticsEvent>(data)));
                        break;
                    case common::MessageType::CreateOrderAckEvent:
                        _strategy.on(common::convert(flatbuffers::GetRoot<schema::CreateOrderAckEvent>(data)));
                        break;
                    case common::MessageType::ModifyOrderAckEvent:
                        _strategy.on(common::convert(flatbuffers::GetRoot<schema::ModifyOrderAckEvent>(data)));
                        break;
                    case common::MessageType::CancelOrderAckEvent:
                        _strategy.on(common::convert(flatbuffers::GetRoot<schema::CancelOrderAckEvent>(data)));
                        break;
                    case common::MessageType::OrderUpdateEvent:
                        _strategy.on(common::convert(flatbuffers::GetRoot<schema::OrderUpdateEvent>(data)));
                        break;
                    case common::MessageType::TradeUpdateEvent:
                        _strategy.on(common::convert(flatbuffers::GetRoot<schema::TradeUpdateEvent>(data)));
                        break;
                    default:
                        LOG(WARNING) << "Unknown message type: " << static_cast<uint8_t>(type);
                }
            }
            catch (...) {
            }
            _buffer.drain(LENGTH_ENVELOPE + length_payload);
        }
    }
}

}  // namespace execution_engine
}  // namespace quinclas
