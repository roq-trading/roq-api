/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <quinclas/tradingapi.h>
#include <list>
#include <unordered_map>
#include <utility>
#include "reference/config.h"
#include "reference/exposure.h"
#include "reference/order.h"
#include "reference/risk_manager.h"

namespace examples {
namespace reference {

class OrderManager final {
  typedef std::chrono::system_clock::time_point time_point_t;

 public:
  // constructor
  OrderManager(const Config& config, const RiskManager& risk_manager,
               quinclas::common::Strategy::Dispatcher& dispatcher);

  // create order (convenience)
  uint32_t buy(const char *order_template, double quantity,
               double limit_price);
  uint32_t sell(const char *order_template, double quantity,
                double limit_price);

  // event handlers
  void on(const quinclas::common::TimerEvent& event);
  void on(const quinclas::common::CreateOrderAckEvent& event);
  void on(const quinclas::common::ModifyOrderAckEvent& event);
  void on(const quinclas::common::CancelOrderAckEvent& event);
  void on(const quinclas::common::OrderUpdateEvent& order_update);

 private:
  uint32_t create_order(const char *order_template,
                        quinclas::common::TradeDirection direction,
                        double quantity, double limit_price);
  void add_timeout_check(uint32_t order_id);
  void check(const quinclas::common::MessageInfo& message_info);

 private:
  const Config& _config;
  const RiskManager& _risk_manager;
  quinclas::common::Strategy::Dispatcher& _dispatcher;
  // consistency check
  time_point_t _last_update_time;
  // order management
  uint32_t _next_order_id = 0;
  std::unordered_map<uint32_t, Order> _orders;
  std::list<std::pair<time_point_t, uint32_t> > _timeout;
  // risk exposure (worst-case)
  Exposure _exposure;
};

}  // namespace reference
}  // namespace examples
