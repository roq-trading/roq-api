/* Copyright (c) 2017,  Hans Erik Thrane */

#pragma once

#include <iostream>

// TODO(thraneh): is_last

#define QUINCLAS_VER "0.1"    ///< Release version
#define QUINCLAS_VER_MAJOR 0  ///< Release version, major number
#define QUINCLAS_VER_MINOR 1  ///< Release version, minor number

namespace quinclas {
namespace common {

/**
 * Constants.
 */
#define MAX_DEPTH 10  ///< Maximum market-by-price depth

/**
 * Typedefs.
 */
typedef uint64_t datetime_t;  // Date time

/**
 * Connection status.
 */
enum class ConnectionStatus : char {
    DISCONNECTED,  ///< Not connected
    CONNECTED      ///< Connected
};

/**
 * Login status.
 */

enum class LoginStatus : char {
    OFF,  ///< Not logged in
    ON    ///< Logged in
};

/**
 * Trade direction.
 */
enum class TradeDirection : uint8_t {
    UNDEFINED,  ///< Undefined
    BUY,        ///< Buy
    SELL        ///< Sell
};

/**
 * Trading status.
 */
enum class TradingStatus : uint8_t {
    UNDEFINED,  ///< Undefined
    CLOSED,     ///< Market is not open for trading
    OPEN        ///< Market is open for trading
};

/**
 * Order status
 */
enum class OrderStatus : uint8_t {
    UNDEFINED,
    SENT,       ///< Order was sent, but no ack has been received yet
    FAILED,     ///< Order was not accepted
    ACCEPTED,   ///< Order has been accepted by the exchange
    CANCELLED,  ///< Order has been cancelled
    FILLED      ///< Order has been completed
};

/**
 * Order book layer (positional by-price aggregated view of an order book).
 */
struct Layer {
    double bid_price;     ///< Bid price
    double bid_quantity;  ///< Aggregated bid quantity
    double ask_price;     ///< Ask price
    double ask_quantity;  ///< Aggregated ask quantity
};

/**
 * Gateway status.
 */
struct GatewayStatus {
    ConnectionStatus market_data_connection_status;       ///< Connection status to market data infrastructure.
    LoginStatus market_data_login_status;                 ///< Login status with market data infrastructure.
    ConnectionStatus order_management_connection_status;  ///< Connection status to order management infrastructure.
    LoginStatus order_management_login_status;            ///< Login status with order management infrastructure.
};

/**
 * Market depth (aggregated view of an order book).
 */
struct MarketByPrice {
    const char *exchange;    ///< Exchange name
    const char *instrument;  ///< Instrument name
    Layer depth[MAX_DEPTH];  ///< Layers
};

/**
 * Session statistics.
 */
struct SessionStatistics {
    const char *exchange;    ///< Exchange name
    const char *instrument;  ///< Instrument name
    double open;             ///< Open price
    double high;             ///< Highest traded price
    double low;              ///< Lowest traded price
};

/**
 * Daily market statistics.
 */
struct DailyStatistics {
    const char *exchange;    ///< Exchange name
    const char *instrument;  ///< Instrument name
    double settlement;       ///< Settlement price
    double open_interest;    ///< Open interest
    double volume;           ///< Total volume
};

/**
 * Reference data.
 */
struct ReferenceData {
    const char *exchange;    ///< Exchange name
    const char *instrument;  ///< Instrument name
    double tick_size;        ///< Tick size (price increments)
    double limit_up;         ///< Price limit-up
    double limit_down;       ///< Price limit-down
};

/**
 * Market status.
 */
struct MarketStatus {
    const char *exchange;          ///< Exchange name
    const char *instrument;        ///< Instrument name
    TradingStatus trading_status;  ///< Market status
};

/**
 * Response to order action.
 */
struct OrderActionAck {
    int opaque;            ///< User provided opaque value (from the original order insert)
    const char *order_id;  ///< Order identifier
};

/**
 * Order update.
 */
struct OrderUpdate {
    const char *exchange;            ///< Exchange name
    const char *instrument;          ///< Instrument name
    const char *order_id;            ///< Order identifier
    OrderStatus status;              ///< Order status
    TradeDirection trade_direction;  ///< Trade direction
    double remaining_quantity;       ///< Remaining (unfilled) quantity
    double traded_quantity;          ///< Traded (filled) quantity
    datetime_t insert_time;          ///< Order insertion time
    datetime_t cancel_time;          ///< Order cancellation time
    const char *order_template;      ///< Order template
    int opaque;                      ///< User provided opaque value (from the original order insert)
};

/**
 * Trade.
 */
struct TradeUpdate {
    const char *exchange;            ///< Exchange name
    const char *instrument;          ///< Instrument name
    const char *order_id;            ///< Order identifier
    const char *trade_id;            ///< Trade identifier
    TradeDirection trade_direction;  ///< Trade direction
    double quantity;                 ///< Trade quantity
    double price;                    ///< Trade price
    int opaque;                      ///< User provided opaque value (from the original order insert)
};


/**
 * Termination event. EXPERIMENTAL
 */
struct TerminationEvent {
    const char *gateway;  ///< Gateway name
};

/**
 * Gateway status update event.
 */
struct GatewayStatusEvent {
    const char *gateway;                  ///< Gateway name
    struct GatewayStatus gateway_status;  ///< Gateway status update
};

/**
 * Reference data update event.
 */
struct ReferenceDataEvent {
    const char *gateway;                  ///< Gateway name
    const ReferenceData& reference_data;  ///< Reference data update
};

/**
 * Market status update event.
 */
struct MarketStatusEvent {
    const char *gateway;                ///< Gateway name
    const MarketStatus& market_status;  ///< Market status update
};

/**
 * Market depth update event.
 */
struct MarketByPriceEvent {
    const char *gateway;                   ///< Gateway name
    const MarketByPrice& market_by_price;  ///< Market-by-price update
};

/**
 * Session statistics update event.
 */
struct SessionStatisticsEvent {
    const char *gateway;                          ///< Gateway name
    const SessionStatistics& session_statistics;  ///< Session statistics update
};

/**
 * Daily statistics update event.
 */
struct DailyStatisticsEvent {
    const char *gateway;                      ///< Gateway name
    const DailyStatistics& daily_statistics;  ///< Daily statistics update
};

/**
 * Order action ack event.
 */
struct OrderActionAckEvent {
    const char *gateway;                     ///< Gateway name
    const OrderActionAck& order_action_ack;  ///< Order action ack
};

/**
 * Order update event.
 */
struct OrderUpdateEvent {
    const char *gateway;              ///< Gateway name
    const OrderUpdate& order_update;  ///< Order update
};

/**
 * Trade update event.
 */
struct TradeUpdateEvent {
    const char *gateway;              ///< Gateway name
    const TradeUpdate& trade_update;  ///< Trade update
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
        virtual void on(const TerminationEvent&) = 0;        ///< Gateway has issued a termination (shutdown) event.
        virtual void on(const GatewayStatusEvent&) = 0;      ///< Connection or login status has changed for a gateway.
        virtual void on(const ReferenceDataEvent&) = 0;      ///< Reference data update for an instrument.
        virtual void on(const MarketStatusEvent&) = 0;       ///< Market status update for an instrument.
        virtual void on(const MarketByPriceEvent&) = 0;      ///< Market-by-price update for an instrument.
        virtual void on(const SessionStatisticsEvent&) = 0;  ///< Session statistics update for an instrument.
        virtual void on(const DailyStatisticsEvent&) = 0;    ///< Daily statistics update for an instrument.
        virtual void on(const OrderActionAckEvent&) = 0;     ///< Response to a create-, modify- or cancel order request.
        virtual void on(const OrderUpdateEvent&) = 0;        ///< New order, or order details have been updated.
        virtual void on(const TradeUpdateEvent&) = 0;        ///< New trade, or trade details have been updated.
    };
    virtual ~Gateway() = default;
    /**
     * Order creation request.
     *
     * @param opaque is...
     * @param exchange is...
     * @param order_template_name is...
     * @param instrument is...
     * @param direction is...
     * @param quantity is...
     * @param limit_price is...
     * @param stop_price is...
     */
    virtual void create_order(
            const int opaque,
            const char *exchange,
            const char *order_template_name,
            const char *instrument,
            const TradeDirection direction,
            const double quantity,
            const double limit_price,
            const double stop_price) = 0;
    /**
     * Order cancellation request.
     *
     * @param opaque is...
     * @param order_id is...
     */
    virtual void cancel_order(const int opaque, const int order_id) = 0;
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
         * Order creation request.
         *
         * @param opaque is...
         * @param gateway is...
         * @param exchange is...
         * @param order_template_name is...
         * @param instrument is...
         * @param direction is...
         * @param quantity is...
         * @param limit_price is...
         * @param stop_price is...
         */
        virtual void create_order(
                const int opaque,
                const char *gateway,
                const char *exchange,
                const char *order_template_name,
                const char *instrument,
                const TradeDirection direction,
                const double quantity,
                const double limit_price,
                const double stop_price) = 0;
        /**
         * Order cancellation request.
         *
         * @param opaque is...
         * @param gateway is...
         * @param order_id is...
         */
        virtual void cancel_order(
                const int opaque,
                const char *gateway,
                const int order_id) = 0;
    };
    virtual ~Strategy() = default;
    virtual void on(const GatewayStatusEvent&) = 0;      ///< Connection or login status has changed for a gateway.
    virtual void on(const ReferenceDataEvent&) = 0;      ///< Reference data update for an instrument.
    virtual void on(const MarketStatusEvent&) = 0;       ///< Market status update for an instrument.
    virtual void on(const MarketByPriceEvent&) = 0;      ///< Market-by-price update for an instrument.
    virtual void on(const SessionStatisticsEvent&) = 0;  ///< Session statistics update for an instrument.
    virtual void on(const DailyStatisticsEvent&) = 0;    ///< Daily statistics update for an instrument.
    virtual void on(const OrderActionAckEvent&) = 0;     ///< Response to a create-, modify- or cancel order request.
    virtual void on(const OrderUpdateEvent&) = 0;        ///< New order, or order details have been updated.
    virtual void on(const TradeUpdateEvent&) = 0;        ///< New trade, or trade details have been updated.
};

}  // namespace common
}  // namespace quinclas

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
        "ask_quantity=" << value.ask_quantity << ", " <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::GatewayStatus& value) {
    return stream << "{"
        "market_data_connection_status=" << value.market_data_connection_status << ", "
        "market_data_login_status=" << value.market_data_login_status << ", "
        "order_management_connection_status=" << value.order_management_connection_status << ", "
        "order_management_login_status=" << value.order_management_login_status <<
        "}";
};

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
            stream << " ";
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

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::GatewayStatusEvent& value) {
    return stream << "{"
        "gateway=\"" << value.gateway << "\", "
        "gateway_status=" << value.gateway_status <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::ReferenceDataEvent& value) {
    return stream << "{"
        "gateway=\"" << value.gateway << "\", "
        "reference_data=" << value.reference_data <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::MarketStatusEvent& value) {
    return stream << "{"
        "gateway=\"" << value.gateway << "\", "
        "market_status=" << value.market_status <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::MarketByPriceEvent& value) {
    return stream << "{"
        "gateway=\"" << value.gateway << "\", "
        "market_by_price=" << value.market_by_price <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::SessionStatisticsEvent& value) {
    return stream << "{"
        "gateway=\"" << value.gateway << "\", "
        "session_statistics=" << value.session_statistics <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::DailyStatisticsEvent& value) {
    return stream << "{"
        "gateway=\"" << value.gateway << "\", "
        "daily_statistics=" << value.daily_statistics <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::OrderUpdateEvent& value) {
    return stream << "{"
        "gateway=\"" << value.gateway << "\", "
        "order_update=" << value.order_update <<
        "}";
}

inline std::ostream& operator<<(std::ostream& stream, const quinclas::common::TradeUpdateEvent& value) {
    return stream << "{"
        "gateway=\"" << value.gateway << "\", "
        "trade_update=" << value.trade_update <<
        "}";
}
