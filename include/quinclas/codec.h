/* Copyright (c) 2017, Hans Erik Thrane */

#pragma once

#include <quinclas/tradingapi.h>
#include <cassert>

namespace quinclas {
namespace common {

enum class MessageType : uint8_t {
    GatewayStatusEvent = 1,
    ReferenceDataEvent = 2,
    MarketStatusEvent = 3,
    MarketByPriceEvent = 4,
    SessionStatisticsEvent = 5,
    DailyStatisticsEvent = 6,
    CreateOrderAckEvent = 7,
    ModifyOrderAckEvent = 8,
    CancelOrderAckEvent = 9,
    OrderUpdateEvent = 10,
    TradeUpdateEvent = 11,
    CreateOrderRequest = 129,
    ModifyOrderRequest = 130,
    CancelOrderRequest = 131,
};

// envelope

class Envelope {
 public:
    static const size_t LENGTH = 4;
    explicit Envelope(const unsigned char *data) :
        _type(static_cast<MessageType>(data[1])),
        _length((static_cast<int>(data[2]) << 8) + static_cast<int>(data[3])) {}
    MessageType type() { return _type; }
    size_t length() { return _length; }
 private:
    MessageType _type;
    size_t _length;
};

// decode (events / low level)

inline common::MessageInfo convert(const schema::MessageInfo *value) {
    return common::MessageInfo{
        .gateway = value->gateway()->c_str(),
        .message_id = value->message_id(),
        .exchange_time = value->exchange_time(),
        .receive_time = value->receive_time(),
        .enqueue_time = value->enqueue_time(),
    };
}

inline common::GatewayStatus convert(const schema::GatewayStatus *value) {
    return common::GatewayStatus{
        .market_data_connection_status = value->market_data_connection_status(),
        .market_data_login_status = value->market_data_login_status(),
        .order_management_connection_status = value->order_management_connection_status(),
        .order_management_login_status = value->order_management_login_status(),
    };
}

inline common::ReferenceData convert(const schema::ReferenceData *value) {
    return common::ReferenceData{
        .exchange = value->exchange()->c_str(),
        .instrument = value->instrument()->c_str(),
        .tick_size = value->tick_size(),
        .limit_up = value->limit_up(),
        .limit_down = value->limit_down(),
    };
}

inline common::MarketStatus convert(const schema::MarketStatus *value) {
    return common::MarketStatus{
        .exchange = value->exchange()->c_str(),
        .instrument = value->instrument()->c_str(),
        .trading_status = value->trading_status(),
    };
}

inline common::Layer convert(const schema::Layer *value) {
    return common::Layer{
        .bid_price = value->bid_price(),
        .bid_quantity = value->bid_quantity(),
        .ask_price = value->ask_price(),
        .ask_quantity = value->ask_quantity(),
    };
}

inline common::MarketByPrice convert(const schema::MarketByPrice *value) {
    common::MarketByPrice res = {
        .exchange = value->exchange()->c_str(),
        .instrument = value->instrument()->c_str(),
    };
    const flatbuffers::Vector<const schema::Layer *> *depth = value->depth();
    assert(depth->Length() == common::MAX_DEPTH);
    for (auto i = 0; i < common::MAX_DEPTH; ++i) {
        res.depth[i] = convert((*depth)[i]);
    }
    return res;
}

inline common::SessionStatistics convert(const schema::SessionStatistics *value) {
    return common::SessionStatistics{
        .exchange = value->exchange()->c_str(),
        .instrument = value->instrument()->c_str(),
        .open = value->open(),
        .high = value->high(),
        .low = value->low(),
    };
}

inline common::DailyStatistics convert(const schema::DailyStatistics *value) {
    return common::DailyStatistics{
        .exchange = value->exchange()->c_str(),
        .instrument = value->instrument()->c_str(),
        .settlement = value->settlement(),
        .open_interest = value->open_interest(),
        .volume = value->volume(),
    };
}

inline common::CreateOrderAck convert(const schema::CreateOrderAck *value) {
    return common::CreateOrderAck{
        .opaque = value->opaque(),
        .order_id = value->order_id()->c_str(),
    };
}

inline common::ModifyOrderAck convert(const schema::ModifyOrderAck *value) {
    return common::ModifyOrderAck{
        .opaque = value->opaque(),
        .order_id = value->order_id()->c_str(),
    };
}

inline common::CancelOrderAck convert(const schema::CancelOrderAck *value) {
    return common::CancelOrderAck{
        .opaque = value->opaque(),
        .order_id = value->order_id()->c_str(),
    };
}

inline common::OrderUpdate convert(const schema::OrderUpdate *value) {
    return common::OrderUpdate{
        .exchange = value->exchange()->c_str(),
        .instrument = value->instrument()->c_str(),
        .order_id = value->order_id()->c_str(),
        .status = value->status(),
        .trade_direction = value->trade_direction(),
        .remaining_quantity = value->remaining_quantity(),
        .traded_quantity = value->traded_quantity(),
        .insert_time = value->insert_time(),
        .cancel_time = value->cancel_time(),
        .order_template = value->order_template()->c_str(),
        .opaque = value->opaque(),
    };
}

inline common::TradeUpdate convert(const schema::TradeUpdate *value) {
    return common::TradeUpdate{
        .exchange = value->exchange()->c_str(),
        .instrument = value->instrument()->c_str(),
        .order_id = value->order_id()->c_str(),
        .trade_id = value->trade_id()->c_str(),
        .trade_direction = value->trade_direction(),
        .quantity = value->quantity(),
        .price = value->price(),
        .opaque = value->opaque(),
    };
}

// decode (requests / low level)

inline common::RequestInfo convert(const schema::RequestInfo *value) {
    return common::RequestInfo{
        .destination = value->destination()->c_str(),
    };
}

inline common::CreateOrder convert(const schema::CreateOrder *value) {
    return common::CreateOrder{
        .exchange = value->exchange()->c_str(),
        .order_template_name = value->order_template_name()->c_str(),
        .instrument = value->instrument()->c_str(),
        .direction = value->direction(),
        .quantity = value->quantity(),
        .limit_price = value->limit_price(),
        .stop_price = value->stop_price(),
        .opaque = value->opaque(),
    };
}

inline common::ModifyOrder convert(const schema::ModifyOrder *value) {
    return common::ModifyOrder{
        .order_id = value->order_id(),
        .opaque = value->opaque(),
    };
}

inline common::CancelOrder convert(const schema::CancelOrder *value) {
    return common::CancelOrder{
        .order_id = value->order_id(),
        .opaque = value->opaque(),
    };
}

// encode (events / low level)

inline flatbuffers::Offset<schema::MessageInfo>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::MessageInfo& value) {
    return schema::CreateMessageInfo(
        fbb,
        fbb.CreateString(value.gateway),
        value.message_id,
        value.exchange_time,
        value.receive_time,
        value.enqueue_time);
}

inline flatbuffers::Offset<schema::GatewayStatus>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::GatewayStatus& value) {
    return schema::CreateGatewayStatus(
        fbb,
        value.market_data_connection_status,
        value.market_data_login_status,
        value.order_management_connection_status,
        value.order_management_login_status);
}

inline flatbuffers::Offset<schema::MarketByPrice>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::MarketByPrice& value) {
    std::vector<schema::Layer> depth(MAX_DEPTH);
    for (auto i = 0; i < MAX_DEPTH; ++i)
        depth[i] = schema::Layer(
            value.depth[i].bid_price,
            value.depth[i].bid_quantity,
            value.depth[i].ask_price,
            value.depth[i].ask_quantity);
    return schema::CreateMarketByPrice(
        fbb,
        fbb.CreateString(value.exchange),
        fbb.CreateString(value.instrument),
        fbb.CreateVectorOfStructs(&depth[0], depth.size()));
}

inline flatbuffers::Offset<schema::SessionStatistics>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::SessionStatistics& value) {
    return schema::CreateSessionStatistics(
        fbb,
        fbb.CreateString(value.exchange),
        fbb.CreateString(value.instrument),
        value.open,
        value.high,
        value.low);
}

inline flatbuffers::Offset<schema::DailyStatistics>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::DailyStatistics& value) {
    return schema::CreateDailyStatistics(
        fbb,
        fbb.CreateString(value.exchange),
        fbb.CreateString(value.instrument),
        value.settlement,
        value.open_interest,
        value.volume);
}

inline flatbuffers::Offset<schema::ReferenceData>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::ReferenceData& value) {
    return schema::CreateReferenceData(
        fbb,
        fbb.CreateString(value.exchange),
        fbb.CreateString(value.instrument),
        value.tick_size,
        value.limit_up,
        value.limit_down);
}

inline flatbuffers::Offset<schema::MarketStatus>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::MarketStatus& value) {
    return schema::CreateMarketStatus(
        fbb,
        fbb.CreateString(value.exchange),
        fbb.CreateString(value.instrument),
        value.trading_status);
}

inline flatbuffers::Offset<schema::CreateOrderAck>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::CreateOrderAck& value) {
    return schema::CreateCreateOrderAck(
        fbb,
        value.opaque,
        fbb.CreateString(value.order_id));
}

inline flatbuffers::Offset<schema::ModifyOrderAck>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::ModifyOrderAck& value) {
    return schema::CreateModifyOrderAck(
        fbb,
        value.opaque,
        fbb.CreateString(value.order_id));
}

inline flatbuffers::Offset<schema::CancelOrderAck>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::CancelOrderAck& value) {
    return schema::CreateCancelOrderAck(
        fbb,
        value.opaque,
        fbb.CreateString(value.order_id));
}

inline flatbuffers::Offset<schema::OrderUpdate>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::OrderUpdate& value) {
    return schema::CreateOrderUpdate(
        fbb,
        fbb.CreateString(value.exchange),
        fbb.CreateString(value.instrument),
        fbb.CreateString(value.order_id),
        value.status,
        value.trade_direction,
        value.remaining_quantity,
        value.traded_quantity,
        value.insert_time,
        value.cancel_time,
        fbb.CreateString(value.order_template),
        value.opaque);
}

inline flatbuffers::Offset<schema::TradeUpdate>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::TradeUpdate& value) {
    return schema::CreateTradeUpdate(
        fbb,
        fbb.CreateString(value.exchange),
        fbb.CreateString(value.instrument),
        fbb.CreateString(value.order_id),
        fbb.CreateString(value.trade_id),
        value.trade_direction,
        value.quantity,
        value.price,
        value.opaque);
}

// encode (events)

inline flatbuffers::Offset<schema::Event>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::GatewayStatusEvent& value) {
    auto message_info = convert(fbb, value.message_info);
    auto gateway_status = convert(fbb, value.gateway_status);
    return schema::CreateEvent(fbb, message_info, schema::EventData::GatewayStatus, gateway_status.Union());
}

inline flatbuffers::Offset<schema::Event>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::ReferenceDataEvent& value) {
    auto message_info = convert(fbb, value.message_info);
    auto reference_data = convert(fbb, value.reference_data);
    return schema::CreateEvent(fbb, message_info, schema::EventData::ReferenceData, reference_data.Union());
}

inline flatbuffers::Offset<schema::Event>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::MarketStatusEvent& value) {
    auto message_info = convert(fbb, value.message_info);
    auto market_status = convert(fbb, value.market_status);
    return schema::CreateEvent(fbb, message_info, schema::EventData::MarketStatus, market_status.Union());
}

inline flatbuffers::Offset<schema::Event>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::MarketByPriceEvent& value) {
    auto message_info = convert(fbb, value.message_info);
    auto market_by_price = convert(fbb, value.market_by_price);
    return schema::CreateEvent(fbb, message_info, schema::EventData::MarketByPrice, market_by_price.Union());
}

inline flatbuffers::Offset<schema::Event>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::SessionStatisticsEvent& value) {
    auto message_info = convert(fbb, value.message_info);
    auto session_statistics = convert(fbb, value.session_statistics);
    return schema::CreateEvent(fbb, message_info, schema::EventData::SessionStatistics, session_statistics.Union());
}

inline flatbuffers::Offset<schema::Event>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::DailyStatisticsEvent& value) {
    auto message_info = convert(fbb, value.message_info);
    auto daily_statistics = convert(fbb, value.daily_statistics);
    return schema::CreateEvent(fbb, message_info, schema::EventData::DailyStatistics, daily_statistics.Union());
}

inline flatbuffers::Offset<schema::Event>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::CreateOrderAckEvent& value) {
    auto message_info = convert(fbb, value.message_info);
    auto create_order_ack = convert(fbb, value.create_order_ack);
    return schema::CreateEvent(fbb, message_info, schema::EventData::CreateOrderAck, create_order_ack.Union());
}

inline flatbuffers::Offset<schema::Event>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::ModifyOrderAckEvent& value) {
    auto message_info = convert(fbb, value.message_info);
    auto modify_order_ack = convert(fbb, value.modify_order_ack);
    return schema::CreateEvent(fbb, message_info, schema::EventData::ModifyOrderAck, modify_order_ack.Union());
}

inline flatbuffers::Offset<schema::Event>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::CancelOrderAckEvent& value) {
    auto message_info = convert(fbb, value.message_info);
    auto cancel_order_ack = convert(fbb, value.cancel_order_ack);
    return schema::CreateEvent(fbb, message_info, schema::EventData::CancelOrderAck, cancel_order_ack.Union());
}

inline flatbuffers::Offset<schema::Event>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::OrderUpdateEvent& value) {
    auto message_info = convert(fbb, value.message_info);
    auto order_update = convert(fbb, value.order_update);
    return schema::CreateEvent(fbb, message_info, schema::EventData::OrderUpdate, order_update.Union());
}

inline flatbuffers::Offset<schema::Event>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::TradeUpdateEvent& value) {
    auto message_info = convert(fbb, value.message_info);
    auto trade_update = convert(fbb, value.trade_update);
    return schema::CreateEvent(fbb, message_info, schema::EventData::TradeUpdate, trade_update.Union());
}

// encode (requests / low level)

inline flatbuffers::Offset<schema::RequestInfo>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::RequestInfo& value) {
    return schema::CreateRequestInfo(fbb, fbb.CreateString(value.destination));
}

inline flatbuffers::Offset<schema::CreateOrder>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::CreateOrder& value) {
    return schema::CreateCreateOrder(
            fbb,
            fbb.CreateString(value.exchange),
            fbb.CreateString(value.order_template_name),
            fbb.CreateString(value.instrument),
            value.direction,
            value.quantity,
            value.limit_price,
            value.stop_price,
            value.opaque);
}

inline flatbuffers::Offset<schema::ModifyOrder>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::ModifyOrder& value) {
    return schema::CreateModifyOrder(fbb, value.order_id, value.opaque);
}

inline flatbuffers::Offset<schema::CancelOrder>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::CancelOrder& value) {
    return schema::CreateCancelOrder(fbb, value.order_id, value.opaque);
}

// encode (requests)

inline flatbuffers::Offset<schema::Request>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::CreateOrderRequest& value) {
    auto request_info = convert(fbb, value.request_info);
    auto create_order = convert(fbb, value.create_order);
    return schema::CreateRequest(fbb, request_info, schema::RequestData::CreateOrder, create_order.Union());
}

inline flatbuffers::Offset<schema::Request>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::ModifyOrderRequest& value) {
    auto request_info = convert(fbb, value.request_info);
    auto modify_order = convert(fbb, value.modify_order);
    return schema::CreateRequest(fbb, request_info, schema::RequestData::ModifyOrder, modify_order.Union());
}

inline flatbuffers::Offset<schema::Request>
convert(flatbuffers::FlatBufferBuilder& fbb, const common::CancelOrderRequest& value) {
    auto request_info = convert(fbb, value.request_info);
    auto cancel_order = convert(fbb, value.cancel_order);
    return schema::CreateRequest(fbb, request_info, schema::RequestData::CancelOrder, cancel_order.Union());
}

}  // namespace execution_engine
}  // namespace quinclas
