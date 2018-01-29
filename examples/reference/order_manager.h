/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <quinclas/tradingapi.h>
#include <list>
#include <unordered_map>
#include <utility>
#include "reference/config.h"
#include "reference/risk_manager.h"

namespace examples {
namespace reference {

class OrderManager final {
 public:
  // constructor
  OrderManager(const Config& config, const RiskManager& risk_manager,
               quinclas::common::Strategy::Dispatcher& dispatcher);

 public:
  // event handlers
  void on(const quinclas::common::TimerEvent& event);
  void on(const quinclas::common::CreateOrderAckEvent& event);
  void on(const quinclas::common::ModifyOrderAckEvent& event);
  void on(const quinclas::common::CancelOrderAckEvent& event);
  void on(const quinclas::common::OrderUpdateEvent& order_update);

 public:
  // create order (easy version)
  bool buy_ioc(double quantity, double limit_price);
  bool sell_ioc(double quantity, double limit_price);

 private:
  // create order (internal version)
  bool create_order(const char *order_template,
                    quinclas::common::TradeDirection direction,
                    double quantity, double limit_price);

 private:
  // utilities
  double get_exposure(quinclas::common::TradeDirection direction) const;

 private:
  const Config& _config;
  const RiskManager& _risk_manager;
  quinclas::common::Strategy::Dispatcher& _dispatcher;

 private:
  // order management
  uint32_t _next_internal_id = 0;
  class Order {
   public:
    Order(quinclas::common::TradeDirection direction, double quantity)
        : direction(direction), remaining_quantity(quantity) {}
    const quinclas::common::TradeDirection direction;
    enum { Requested, Sent, Working } state = Requested;
    double remaining_quantity;
    uint32_t gateway_order_id = 0;
  };
  std::unordered_map<uint32_t, Order> _orders;
  typedef std::chrono::system_clock::time_point time_point_t;
  std::list<std::pair<time_point_t, uint32_t> > _timeout;

 private:
  // consistency (particularly useful for simulation)
  time_point_t _last_update_time;
  void check(const quinclas::common::MessageInfo& message_info);
};

}  // namespace reference
}  // namespace examples

