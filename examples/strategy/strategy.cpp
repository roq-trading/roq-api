/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "strategy/strategy.h"
#include <glog/logging.h>

#include <limits>
#include <string>

// Convenience (and not liked by cpplint).
using namespace quinclas::common;  // NOLINT

namespace examples {
namespace strategy {

Strategy::Strategy(Strategy::Dispatcher& dispatcher, uint32_t ticks_to_trade)
    : _dispatcher(dispatcher),
      _ticks_to_trade(ticks_to_trade) {}

void Strategy::on(const TimerEvent& event) {
  // You can only use timer events to guarantee a certain point in time has
  // been reached.
  // For example, you may decide a submitted order has timed out.
  // However, do not rely on the accuracy or frequency of the timer!

  // Example:
  LOG(INFO) << "TimerEvent={"
      "current_utc_time=" << event.timer.utc_time <<
      "}";
}

void Strategy::on(const IdleEvent&) {
  // You can use idle events to process updates in batch.
  // For example, the inbound queue may contain multiple updates to the same
  // market depth.
  // Your implementation strategy could be such that you set a dirty flag and
  // otherwise only make a copy each time you receive the market depth update
  // event (perhaps also do light-weight processing).
  // You can then use this idle event to detect market depth has been changed
  // and then begin heavy-weight processing knowing that you can't possibly
  // fall behind.
  // The idle event is therefore your tool to implement flow-control (e.g.
  // conflation as just described in the example).

  // Example:
  // The event has no associated information!
}

void Strategy::on(const GatewayStatusEvent& event) {
  LOG(INFO) << "GatewayStatusEvent=" << event;
  // Every change to gateway connection and/or login status will be
  // communicated here.
  // Changes include;
  // - Gateway is unable to connect to broker and/or market
  // - Gateway was disconnected from broker and/or market
  // - Gateway user has been logged in
  // - Gateway user has (remotely) been logged out
  // - This application being disconnected from the gateway

  // Example:
  const auto& gateway_status = event.gateway_status;
  // Check if we're ready to trade.
  bool market_data_ready =
      gateway_status.market_data_connection_status == ConnectionStatus::Connected &&
      gateway_status.market_data_login_status == LoginStatus::On;
  bool order_management_ready =
      gateway_status.order_management_connection_status == ConnectionStatus::Connected &&
      gateway_status.order_management_login_status == LoginStatus::On;
  bool ready = market_data_ready && order_management_ready;
  // Note!
  // You don't actually have to check both connection and login status.
  // It is enough to check the login status.
  // The controller and the gateway will guarantee you can't be logged in
  // and *not* connected at the same time.
  // ... this achieves the same;
  ready =
      gateway_status.market_data_login_status == LoginStatus::On &&
      gateway_status.order_management_login_status == LoginStatus::On;
  // Return if not ready yet.
  if (!ready)
    return;
  // Construct a create-order request.
  CreateOrder create_order {
    .opaque         = 1,  // this ID can be used to identify your order
    .order_template = "ioc_open",
    .exchange       = "CFFEX",
    .instrument     = "IF1802",
    .direction      = TradeDirection::Buy,
    .quantity       = 1,
    .limit_price    = 0.01,
    .stop_price     = std::numeric_limits<double>::quiet_NaN(),
  };
  // Send the request to the FEMAS gateway.
  _dispatcher.send("FEMAS", create_order);
  // Note!
  // The name of the gateway is the name you choose when constructing
  // the controller framework (see: main.cpp).
}

void Strategy::on(const ReferenceDataEvent& event) {
  LOG(INFO) << "ReferenceDataEvent=" << event;

  // Example:
  const auto& reference_data = event.reference_data;
  // Return if the update is for something else than what we care about.
  if (0 != strcmp(reference_data.instrument, "IF1802"))
    return;
  // Maybe we need the tick-size?
  double tick_size = reference_data.tick_size;
}

void Strategy::on(const MarketStatusEvent& event) {
  LOG(INFO) << "MarketStatusEvent=" << event;

  // Example:
  const auto& market_status = event.market_status;
  // Return if the update is for something else than what we care about.
  if (0 != strcmp(market_status.instrument, "IF1802"))
    return;
  // Ready for trading?
  bool ready = market_status.trading_status == TradingStatus::Open;
}

void Strategy::on(const MarketByPriceEvent& event) {
  LOG(INFO) << "MarketByPriceEvent=" << event;

  // Example:
  const auto& market_by_price = event.market_by_price;
  // Return if the update is for something else than what we care about.
  if (0 != strcmp(market_by_price.instrument, "IF1802"))
    return;
  // Compute weighted mid based on top of book.
  const auto& depth = market_by_price.depth;
  const auto& top_of_book = depth[0];
  double weighted_mid =
    (top_of_book.bid_price * top_of_book.bid_quantity +
     top_of_book.ask_price * top_of_book.ask_quantity) /
    (top_of_book.bid_quantity + top_of_book.ask_quantity);
}

void Strategy::on(const SessionStatisticsEvent& event) {
  LOG(INFO) << "SessionStatisticsEvent=" << event;

  // Example:
  const auto& session_statistics = event.session_statistics;
  // Return if the update is for something else than what we care about.
  if (0 != strcmp(session_statistics.instrument, "IF1802"))
    return;
  // Maybe we need the open price?
  double open = session_statistics.open;
}

void Strategy::on(const DailyStatisticsEvent& event) {
  LOG(INFO) << "DailyStatisticsEvent=" << event;

  // Example:
  const auto& daily_statistics = event.daily_statistics;
  // Return if the update is for something else than what we care about.
  if (0 != strcmp(daily_statistics.instrument, "IF1802"))
    return;
  // Maybe we need the volume?
  double volume = daily_statistics.volume;
}

void Strategy::on(const CreateOrderAckEvent& event) {
  LOG(INFO) << "CreateOrderAckEvent=" << event;

  // Example:
  const auto& create_order_ack = event.create_order_ack;
  // Check our pass-through ID.
  if (create_order_ack.opaque == 1) {
    // It can be used to identify and/or look up a specific order.
  } else {
    // This scenario is not expected...
  }
  if (create_order_ack.failure) {
    // Somthing went wrong -- the order was not created...
  }
  // Note!
  // For various reasons (e.g. network issues), an ack event is *not*
  // guaranteed!
  // If you're working with resting orders, you *must* used the TimerEvent
  // to check for such scenarios.
  // You should *never* assume an order actually reaches the market unless you
  // receive the confirmation.
}

void Strategy::on(const ModifyOrderAckEvent& event) {
  LOG(INFO) << "ModifyOrderAckEvent=" << event;

  // Example:
  const auto& modify_order_ack = event.modify_order_ack;
  // Same usage and comments as above (see: CreateOrderAckEvent).
}

void Strategy::on(const CancelOrderAckEvent& event) {
  LOG(INFO) << "CancelOrderAckEvent=" << event;

  // Example:
  const auto& cancel_order_ack = event.cancel_order_ack;
  // Same usage and comments as above (see: CreateOrderAckEvent).
}

void Strategy::on(const OrderUpdateEvent& event) {
  LOG(INFO) << "OrderUpdateEvent=" << event;

  // Example:
  const auto& order_update = event.order_update;
  // Check our pass-through ID.
  if (order_update.opaque == 1) {
    // It can be used to identify and/or look up a specific order.
  } else {
    // This scenario is not expected...
  }
  // Check the order status.
  switch (order_update.status) {
    case OrderStatus::Undefined:
      // Here you should probably abort and investigate!
      break;
    case OrderStatus::Sent:
      break;
    case OrderStatus::Failed:
      break;
    case OrderStatus::Accepted:
      break;
    case OrderStatus::Cancelled:
      if (0 < static_cast<uint32_t>(order_update.remaining_quantity)) {
        // Should probably deal with remaining quantity.
      }
      break;
    case OrderStatus::Filled:
      if (0 == static_cast<uint32_t>(order_update.remaining_quantity)) {
        // We're done -- there is no remaining quantity.
      }
      break;
  }
}

void Strategy::on(const TradeUpdateEvent& event) {
  LOG(INFO) << "TradeUpdateEvent=" << event;

  // Example:
  const auto& trade_update = event.trade_update;
  // Avoid double-counting trades by checking the external trade id.
  std::string trade_id(trade_update.external_trade_id);
  // You should accumulate positions and P&L by instrument id.
  std::string instrument(trade_update.instrument);
  // Trade direction.
  double factor;
  switch (trade_update.trade_direction) {
    case TradeDirection::Undefined:
      // Here you should probably abort and investigate!
      break;
    case TradeDirection::Buy:
      factor = 1.0;
      break;
    case TradeDirection::Sell:
      factor = -1.0;
      break;
  }
  // Traded quantity (always positive!).
  double quantity = trade_update.quantity;
  // Traded price.
  double price = trade_update.price;
}

}  // namespace strategy
}  // namespace examples
