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
                const auto root = flatbuffers::GetRoot<schema::Event>(data);
                const auto message_info = common::convert(root->message_info());
                const auto type = root->event_data_type();
                switch (type) {
                    case schema::EventData::GatewayStatus:
                            {
                            const auto gateway_status = common::convert(root->event_data_as_GatewayStatus());
                            const auto event = common::GatewayStatusEvent{
                                .message_info = message_info,
                                .gateway_status = gateway_status};
                            _strategy.on(event);
                            }
                        break;
                    case schema::EventData::ReferenceData:
                            {
                            const auto reference_data = common::convert(root->event_data_as_ReferenceData());
                            const auto event = common::ReferenceDataEvent{
                                .message_info = message_info,
                                .reference_data = reference_data};
                            _strategy.on(event);
                            }
                        break;
                    case schema::EventData::MarketStatus:
                            {
                            const auto market_status = common::convert(root->event_data_as_MarketStatus());
                            const auto event = common::MarketStatusEvent{
                                .message_info = message_info,
                                .market_status = market_status};
                            _strategy.on(event);
                            }
                        break;
                    case schema::EventData::MarketByPrice:
                            {
                            const auto market_by_price = common::convert(root->event_data_as_MarketByPrice());
                            const auto event = common::MarketByPriceEvent{
                                .message_info = message_info,
                                .market_by_price = market_by_price};
                            _strategy.on(event);
                            }
                        break;
                    case schema::EventData::SessionStatistics:
                            {
                            const auto session_statistics = common::convert(root->event_data_as_SessionStatistics());
                            const auto event = common::SessionStatisticsEvent{
                                .message_info = message_info,
                                .session_statistics = session_statistics};
                            _strategy.on(event);
                            }
                        break;
                    case schema::EventData::DailyStatistics:
                            {
                            const auto daily_statistics = common::convert(root->event_data_as_DailyStatistics());
                            const auto event = common::DailyStatisticsEvent{
                                .message_info = message_info,
                                .daily_statistics = daily_statistics};
                            _strategy.on(event);
                            }
                        break;
                    case schema::EventData::CreateOrderAck:
                            {
                            const auto create_order_ack = common::convert(root->event_data_as_CreateOrderAck());
                            const auto event = common::CreateOrderAckEvent{
                                .message_info = message_info,
                                .create_order_ack = create_order_ack};
                            _strategy.on(event);
                            }
                        break;
                    case schema::EventData::ModifyOrderAck:
                            {
                            const auto modify_order_ack = common::convert(root->event_data_as_ModifyOrderAck());
                            const auto event = common::ModifyOrderAckEvent{
                                .message_info = message_info,
                                .modify_order_ack = modify_order_ack};
                            _strategy.on(event);
                            }
                        break;
                    case schema::EventData::CancelOrderAck:
                            {
                            const auto cancel_order_ack = common::convert(root->event_data_as_CancelOrderAck());
                            const auto event = common::CancelOrderAckEvent{
                                .message_info = message_info,
                                .cancel_order_ack = cancel_order_ack};
                            _strategy.on(event);
                            }
                        break;
                    case schema::EventData::OrderUpdate:
                            {
                            const auto order_update = common::convert(root->event_data_as_OrderUpdate());
                            const auto event = common::OrderUpdateEvent{
                                .message_info = message_info,
                                .order_update = order_update};
                            _strategy.on(event);
                            }
                        break;
                    case schema::EventData::TradeUpdate:
                            {
                            const auto trade_update = common::convert(root->event_data_as_TradeUpdate());
                            const auto event = common::TradeUpdateEvent{
                                .message_info = message_info,
                                .trade_update = trade_update};
                            _strategy.on(event);
                            }
                        break;
                    default:
                        if (schema::EventData::MAX < type)
                            LOG(WARNING) << "Unknown message type: " << static_cast<uint8_t>(type);
                        else
                            LOG(WARNING) << "Unknown message type: " << schema::EnumNamesEventData()[static_cast<uint8_t>(type)];
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
