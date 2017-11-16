/* Copyright (c) 2017,  Hans Erik Thrane */

#pragma once

// TODO(thraneh): share enum class definitions (drop here)
#include "tradingapi_generated.h"
#include <iostream>

#define QUINCLAS_VERSION "0.1"                               ///< Version

namespace quinclas {
namespace common {

/**
 * Constants.
 */
#define MAX_DEPTH 10                                         ///< Maximum market-by-price depth

/**
 * Typedefs.
 */
typedef uint64_t datetime_t;                                 // Date time

/**
 * Connection status.
 */
enum class ConnectionStatus : char {
    DISCONNECTED,                                            ///< Not connected
    CONNECTED                                                ///< Connected
};

/**
 * Login status.
 */
enum class LoginStatus : char {
    OFF,                                                     ///< Not logged in
    ON                                                       ///< Logged in
};

/**
 * Trade direction.
 */
enum class TradeDirection : uint8_t {
    UNDEFINED,                                               ///< Undefined
    BUY,                                                     ///< Buy
    SELL                                                     ///< Sell
};

/**
 * Trading status.
 */
enum class TradingStatus : uint8_t {
    UNDEFINED,                                               ///< Undefined
    CLOSED,                                                  ///< Market is not open for trading
    OPEN                                                     ///< Market is open for trading
};

/**
 * Order status
 */
enum class OrderStatus : uint8_t {
    UNDEFINED,
    SENT,                                                    ///< Order was sent, but no ack has been received yet
    FAILED,                                                  ///< Order was not accepted
    ACCEPTED,                                                ///< Order has been accepted by the exchange
    CANCELLED,                                               ///< Order has been cancelled
    FILLED                                                   ///< Order has been completed
};

/**
 * Order book layer (positional by-price aggregated view of an order book).
 */
struct Layer {
    double bid_price;                                        ///< Bid price
    double bid_quantity;                                     ///< Aggregated bid quantity
    double ask_price;                                        ///< Ask price
    double ask_quantity;                                     ///< Aggregated ask quantity
};

/**
 * Gateway status.
 */
struct GatewayStatus {
    ConnectionStatus market_data_connection_status;          ///< Connection status to market data infrastructure
    LoginStatus market_data_login_status;                    ///< Login status with market data infrastructure
    ConnectionStatus order_management_connection_status;     ///< Connection status to order management infrastructure
    LoginStatus order_management_login_status;               ///< Login status with order management infrastructure
};

/**
 * Market depth (aggregated view of an order book).
 */
struct MarketByPrice {
    const char *exchange;                                    ///< Exchange name
    const char *instrument;                                  ///< Instrument name
    Layer depth[MAX_DEPTH];                                  ///< Layers
};

/**
 * Session statistics.
 */
struct SessionStatistics {
    const char *exchange;                                    ///< Exchange name
    const char *instrument;                                  ///< Instrument name
    double open;                                             ///< Open price
    double high;                                             ///< Highest traded price
    double low;                                              ///< Lowest traded price
};

/**
 * Daily market statistics.
 */
struct DailyStatistics {
    const char *exchange;                                    ///< Exchange name
    const char *instrument;                                  ///< Instrument name
    double settlement;                                       ///< Settlement price
    double open_interest;                                    ///< Open interest
    double volume;                                           ///< Total volume
};

/**
 * Reference data.
 */
struct ReferenceData {
    const char *exchange;                                    ///< Exchange name
    const char *instrument;                                  ///< Instrument name
    double tick_size;                                        ///< Tick size (price increments)
    double limit_up;                                         ///< Price limit-up
    double limit_down;                                       ///< Price limit-down
};

/**
 * Market status.
 */
struct MarketStatus {
    const char *exchange;                                    ///< Exchange name
    const char *instrument;                                  ///< Instrument name
    TradingStatus trading_status;                            ///< Market status
};

/**
 * Response to create order
 */
struct CreateOrderAck {
    int opaque;                                              ///< User provided opaque value
    const char *order_id;                                    ///< Order identifier
};

/**
 * Response to modify order
 */
struct ModifyOrderAck {
    int opaque;                                              ///< User provided opaque value
    const char *order_id;                                    ///< Order identifier
};

/**
 * Response to cancel order
 */
struct CancelOrderAck {
    int opaque;                                              ///< User provided opaque value
    const char *order_id;                                    ///< Order identifier
};

/**
 * Order update.
 */
struct OrderUpdate {
    const char *exchange;                                    ///< Exchange name
    const char *instrument;                                  ///< Instrument name
    const char *order_id;                                    ///< Order identifier
    OrderStatus status;                                      ///< Order status
    TradeDirection trade_direction;                          ///< Trade direction
    double remaining_quantity;                               ///< Remaining (unfilled) quantity
    double traded_quantity;                                  ///< Traded (filled) quantity
    datetime_t insert_time;                                  ///< Order insertion time
    datetime_t cancel_time;                                  ///< Order cancellation time
    const char *order_template;                              ///< Order template
    int opaque;                                              ///< User provided opaque value
};

/**
 * Trade.
 */
struct TradeUpdate {
    const char *exchange;                                    ///< Exchange name
    const char *instrument;                                  ///< Instrument name
    const char *order_id;                                    ///< Order identifier
    const char *trade_id;                                    ///< Trade identifier
    TradeDirection trade_direction;                          ///< Trade direction
    double quantity;                                         ///< Trade quantity
    double price;                                            ///< Trade price
    int opaque;                                              ///< User provided opaque value
};

/**
 * Termination event. EXPERIMENTAL
 */
struct TerminationEvent {
    const char *gateway;                                     ///< Gateway name
};

/**
 * MessageInfo.
 */
struct MessageInfo {
    const char *gateway;                                     ///< Gateway name
    uint64_t message_id;                                     ///< Message identifier
    datetime_t exchange_time;                                ///< Exchange timestamp
    datetime_t receive_time;                                 ///< Receive timestamp
    datetime_t enqueue_time;                                 ///< Enqueue timestamp
};

/**
 * Idle event.
 */
struct IdleEvent {};

/**
 * Gateway status update event.
 */
struct GatewayStatusEvent {
    const MessageInfo& message_info;                         ///< Message info
    const GatewayStatus& gateway_status;                     ///< Gateway status update
};

/**
 * Reference data update event.
 */
struct ReferenceDataEvent {
    const MessageInfo& message_info;                         ///< Message info
    const ReferenceData& reference_data;                     ///< Reference data update
};

/**
 * Market status update event.
 */
struct MarketStatusEvent {
    const MessageInfo& message_info;                         ///< Message info
    const MarketStatus& market_status;                       ///< Market status update
};

/**
 * Market depth update event.
 */
struct MarketByPriceEvent {
    const MessageInfo& message_info;                         ///< Message info
    const MarketByPrice& market_by_price;                    ///< Market-by-price update
};

/**
 * Session statistics update event.
 */
struct SessionStatisticsEvent {
    const MessageInfo& message_info;                         ///< Message info
    const SessionStatistics& session_statistics;             ///< Session statistics update
};

/**
 * Daily statistics update event.
 */
struct DailyStatisticsEvent {
    const MessageInfo& message_info;                         ///< Message info
    const DailyStatistics& daily_statistics;                 ///< Daily statistics update
};

/**
 * Create order ack event.
 */
struct CreateOrderAckEvent {
    const MessageInfo& message_info;                         ///< Message info
    const CreateOrderAck& create_order_ack;                  ///< Create order ack
};

/**
 * Modify order ack event.
 */
struct ModifyOrderAckEvent {
    const MessageInfo& message_info;                         ///< Message info
    const ModifyOrderAck& modify_order_ack;                  ///< Modify order ack
};

/**
 * Cancel order ack event.
 */
struct CancelOrderAckEvent {
    const MessageInfo& message_info;                         ///< Message info
    const CancelOrderAck& cancel_order_ack;                  ///< Cancel order ack
};

/**
 * Order update event.
 */
struct OrderUpdateEvent {
    const MessageInfo& message_info;                         ///< Message info
    const OrderUpdate& order_update;                         ///< Order update
};

/**
 * Trade update event.
 */
struct TradeUpdateEvent {
    const MessageInfo& message_info;                         ///< Message info
    const TradeUpdate& trade_update;                         ///< Trade update
};

/**
 * Create order.
 */
struct CreateOrder {
    const char *exchange;                                    ///< Exchange name
    const char *order_template_name;                         ///< Order template name
    const char *instrument;                                  ///< Instrument name
    const TradeDirection direction;                          ///< Trade direction
    const double quantity;                                   ///< Desired quantity
    const double limit_price;                                ///< Limit price
    const double stop_price;                                 ///< Stop price
    const int opaque;                                        ///< Opaque pass-through value
};

/**
 * Modify order.
 */
struct ModifyOrder {
    const int order_id;                                      ///< Order id
    const int opaque;                                        ///< Opaque pass-through value
};

/**
 * Cancel order.
 */
struct CancelOrder {
    const int order_id;                                      ///< Order id
    const int opaque;                                        ///< Opaque pass-through value
};

/**
 * RequestInfo.
 */
struct RequestInfo {
    const char *destination;                                 ///< Destination name, typically a gateway
};

/*
 * Create order request.
 */

struct CreateOrderRequest {
    const RequestInfo& request_info;                         ///< Routing information
    const CreateOrder& create_order;                         ///< Create order details
};

/*
 * Modify order request.
 */

struct ModifyOrderRequest {
    const RequestInfo& request_info;                         ///< Routing information
    const ModifyOrder& modify_order;                         ///< Modify order details
};

/*
 * Cancel order request.
 */

struct CancelOrderRequest {
    const RequestInfo& request_info;                         ///< Routing information
    const CancelOrder& cancel_order;                         ///< Cancel order details
};

/**
 * Gateway interface.
 */
class Gateway {
 public:
     /**
      * Callback handler interface.
      */
    class Handler {
     public:
        virtual void on(const TerminationEvent&) = 0;        ///< Gateway has issued a termination (shutdown) event
        virtual void on(const GatewayStatusEvent&) = 0;      ///< Connection or login status has changed for a gateway
        virtual void on(const ReferenceDataEvent&) = 0;      ///< Reference data update for an instrument
        virtual void on(const MarketStatusEvent&) = 0;       ///< Market status update for an instrument
        virtual void on(const MarketByPriceEvent&) = 0;      ///< Market-by-price update for an instrument
        virtual void on(const SessionStatisticsEvent&) = 0;  ///< Session statistics update for an instrument
        virtual void on(const DailyStatisticsEvent&) = 0;    ///< Daily statistics update for an instrument
        virtual void on(const CreateOrderAckEvent&) = 0;     ///< Response to a create order request
        virtual void on(const ModifyOrderAckEvent&) = 0;     ///< Response to a modify order request
        virtual void on(const CancelOrderAckEvent&) = 0;     ///< Response to a cancel order request
        virtual void on(const OrderUpdateEvent&) = 0;        ///< New order, or order details have been updated
        virtual void on(const TradeUpdateEvent&) = 0;        ///< New trade, or trade details have been updated
    };
    virtual ~Gateway() = default;
    /**
     * Start the event loop.
     */
    virtual void start() = 0;
    /**
     * Send a CreateOrder request.
     *
     * @param create_order_request has the order creation request details.
     */
    virtual void send(const CreateOrderRequest& create_order_request) = 0;
    /**
     * Send a ModifyOrder request.
     *
     * @param modify_order_request has the order modification request details.
     */
    virtual void send(const ModifyOrderRequest& modify_order_request) = 0;
    /**
     * Send a CancelOrder request.
     *
     * @param cancel_order_request has the order cancellation request details.
     */
    virtual void send(const CancelOrderRequest& cancel_order_request) = 0;
};

/**
 * Strategy interface.
 */
class Strategy {
 public:
     /*
      * Dispatcher interface.
      */
    class Dispatcher {
     public:
        /**
         * Send CreateOrder request.
         *
         * @param create_order_request has the order creation request details.
         */
        virtual void send(const CreateOrderRequest& create_order_request) = 0;
        /**
         * Send ModifyOrder request.
         *
         * @param modify_order_request has the order modification request details.
         */
        virtual void send(const ModifyOrderRequest& modify_order_request) = 0;
        /**
         * Send CancelOrder request.
         *
         * @param cancel_order_request has the order cancellation request details.
         */
        virtual void send(const CancelOrderRequest& cancel_order_request) = 0;
    };
    virtual ~Strategy() = default;
    virtual void on(const IdleEvent&) = 0;                   ///< Idle
    virtual void on(const GatewayStatusEvent&) = 0;          ///< Connection or login status has changed for a gateway
    virtual void on(const ReferenceDataEvent&) = 0;          ///< Reference data update for an instrument
    virtual void on(const MarketStatusEvent&) = 0;           ///< Market status update for an instrument
    virtual void on(const MarketByPriceEvent&) = 0;          ///< Market-by-price update for an instrument
    virtual void on(const SessionStatisticsEvent&) = 0;      ///< Session statistics update for an instrument
    virtual void on(const DailyStatisticsEvent&) = 0;        ///< Daily statistics update for an instrument
    virtual void on(const CreateOrderAckEvent&) = 0;         ///< Response to a create order request
    virtual void on(const ModifyOrderAckEvent&) = 0;         ///< Response to a modify order request
    virtual void on(const CancelOrderAckEvent&) = 0;         ///< Response to a cancel order request
    virtual void on(const OrderUpdateEvent&) = 0;            ///< New order, or order details have been updated
    virtual void on(const TradeUpdateEvent&) = 0;            ///< New trade, or trade details have been updated
};

}  // namespace common
}  // namespace quinclas

extern "C" {
typedef const char *(*GetVersion)();
typedef quinclas::common::Gateway *(*Create)(quinclas::common::Gateway::Handler& handler, const char *config_file);
typedef void (*Destroy)(quinclas::common::Gateway *gateway);
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::ConnectionStatus value) {
    switch (value) {
        case quinclas::common::ConnectionStatus::CONNECTED: return stream << "CONNECTED";
        case quinclas::common::ConnectionStatus::DISCONNECTED: return stream << "DISCONNECTED";
        default: return stream << "UNKNOWN:" << static_cast<int>(value);
    }
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::LoginStatus value) {
    switch (value) {
        case quinclas::common::LoginStatus::OFF: return stream << "OFF";
        case quinclas::common::LoginStatus::ON: return stream << "ON";
        default: return stream << "UNKNOWN:" << static_cast<int>(value);
    }
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::TradeDirection value) {
    switch (value) {
        case quinclas::common::TradeDirection::BUY: return stream << "BUY";
        case quinclas::common::TradeDirection::SELL: return stream << "SELL";
        default: return stream << "UNKNOWN:" << static_cast<int>(value);
    }
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::TradingStatus value) {
    switch (value) {
        case quinclas::common::TradingStatus::OPEN: return stream << "OPEN";
        case quinclas::common::TradingStatus::CLOSED: return stream << "CLOSED";
        default: return stream << "UNKNOWN:" << static_cast<int>(value);
    }
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::OrderStatus value) {
    switch (value) {
        case quinclas::common::OrderStatus::UNDEFINED: return stream << "UNDEFINED";
        case quinclas::common::OrderStatus::SENT: return stream << "SENT";
        case quinclas::common::OrderStatus::FAILED: return stream << "FAILED";
        case quinclas::common::OrderStatus::ACCEPTED: return stream << "ACCEPTED";
        case quinclas::common::OrderStatus::CANCELLED: return stream << "CANCELLED";
        case quinclas::common::OrderStatus::FILLED: return stream << "FILLED";
        default: return stream << "UNKNOWN:" << static_cast<int>(value);
    }
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::Layer& value) {
    return stream << "{"
        "bid_price=" << value.bid_price << ", " <<
        "bid_quantity=" << value.bid_quantity << ", " <<
        "ask_price=" << value.ask_price << ", " <<
        "ask_quantity=" << value.ask_quantity <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::GatewayStatus& value) {
    return stream << "{"
        "market_data_connection_status=" << value.market_data_connection_status << ", "
        "market_data_login_status=" << value.market_data_login_status << ", "
        "order_management_connection_status=" << value.order_management_connection_status << ", "
        "order_management_login_status=" << value.order_management_login_status <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::ReferenceData& value) {
    return stream << "{"
        "exchange=\"" << value.exchange << "\", " <<
        "instrument=\"" << value.exchange << "\", " <<
        "tick_size=" << value.tick_size << ", "
        "tick_size=" << value.tick_size << ", "
        "limit_up=" << value.limit_up << ", "
        "limit_down=" << value.limit_down <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::MarketStatus& value) {
    return stream << "{"
        "exchange=\"" << value.exchange << "\", " <<
        "instrument=\"" << value.exchange << "\", " <<
        "trading_status=" << value.trading_status <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::MarketByPrice& value) {
    const size_t length = sizeof(value.depth) / sizeof(value.depth[0]);
    stream << "[";
    for (size_t index = 0; index < length; ++index) {
        if (index > 0)
            stream << ", ";
        stream << value.depth[index];
    }
    return stream << "]";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::SessionStatistics& value) {
    return stream << "{"
        "open=" << value.open << ", " <<
        "high=" << value.high << ", " <<
        "low=" << value.low <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::DailyStatistics& value) {
    return stream << "{"
        "settlement=" << value.settlement << ", " <<
        "open_interest=" << value.open_interest << ", " <<
        "volume=" << value.volume <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::OrderUpdate& value) {
    return stream << "{"
        "exchange=\"" << value.exchange << "\", " <<
        "instrument=\"" << value.instrument << "\", " <<
        "order_id=\"" << value.order_id << "\", " <<
        "status=" << value.status << ", "
        "trade_direction=" << value.trade_direction << ", "
        "remaining_quantity=" << value.remaining_quantity << ", "
        "traded_quantity=" << value.traded_quantity << ", "
        "insert_time=" << value.insert_time << ", "
        "cancel_time=" << value.cancel_time << ", "
        "order_template=\"" << value.order_template << "\", " <<
        "opaque=" << value.opaque <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::TradeUpdate& value) {
    return stream << "{"
        "exchange=\"" << value.exchange << "\", " <<
        "instrument=\"" << value.instrument << "\", " <<
        "order_id=\"" << value.order_id << "\", " <<
        "trade_id=\"" << value.trade_id << "\", " <<
        "trade_direction=" << value.trade_direction << ", "
        "remaining_quantity=" << value.quantity << ", "
        "quantity=" << value.quantity << ", "
        "price=" << value.price << ", "
        "opaque=" << value.opaque <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::MessageInfo& value) {
    return stream << "{"
        "gateway=\"" << value.gateway << "\", " <<
        "message_id=" << value.message_id << ", " <<
        "exchange_time=" << value.exchange_time << ", " <<
        "receive_time=" << value.receive_time << ", " <<
        "enqueue_time=" << value.enqueue_time <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::GatewayStatusEvent& value) {
    return stream << "{"
        "message_info=" << value.message_info << ", "
        "gateway_status=" << value.gateway_status <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::ReferenceDataEvent& value) {
    return stream << "{"
        "message_info=" << value.message_info << ", "
        "reference_data=" << value.reference_data <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::MarketStatusEvent& value) {
    return stream << "{"
        "message_info=" << value.message_info << ", "
        "market_status=" << value.market_status <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::MarketByPriceEvent& value) {
    return stream << "{"
        "message_info=" << value.message_info << ", "
        "market_by_price=" << value.market_by_price <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::SessionStatisticsEvent& value) {
    return stream << "{"
        "message_info=" << value.message_info << ", "
        "session_statistics=" << value.session_statistics <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::DailyStatisticsEvent& value) {
    return stream << "{"
        "message_info=" << value.message_info << ", "
        "daily_statistics=" << value.daily_statistics <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::OrderUpdateEvent& value) {
    return stream << "{"
        "message_info=" << value.message_info << ", "
        "order_update=" << value.order_update <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::TradeUpdateEvent& value) {
    return stream << "{"
        "message_info=" << value.message_info << ", "
        "trade_update=" << value.trade_update <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::CreateOrder& value) {
    return stream << "{"
        "exchange=\"" << value.exchange << "\", " <<
        "order_template_name=\"" << value.order_template_name << "\", " <<
        "instrument=\"" << value.instrument << "\", " <<
        "direction=" << value.direction << ", "
        "quantity=" << value.quantity << ", "
        "limit_price=" << value.limit_price << ", "
        "stop_price=" << value.stop_price << ", "
        "opaque=" << value.opaque <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::ModifyOrder& value) {
    return stream << "{"
        "order_id=\"" << value.order_id << "\", " <<
        "opaque=" << value.opaque <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::CancelOrder& value) {
    return stream << "{"
        "order_id=\"" << value.order_id << "\", " <<
        "opaque=" << value.opaque <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::RequestInfo& value) {
    return stream << "{"
        "destination=\"" << value.destination <<
        "}";
}
inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::CreateOrderRequest& value) {
    return stream << "{"
        "request_info=" << value.request_info << ", "
        "create_order=" << value.create_order <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::ModifyOrderRequest& value) {
    return stream << "{"
        "request_info=" << value.request_info << ", "
        "modify_order=" << value.modify_order <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::CancelOrderRequest& value) {
    return stream << "{"
        "request_info=" << value.request_info << ", "
        "cancel_order=" << value.cancel_order <<
        "}";
}
