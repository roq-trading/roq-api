/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <quinclas/tradingapi.h>
#include <list>
#include <unordered_map>
#include <utility>
#include "reference/config.h"
#include "reference/order.h"
#include "reference/risk_manager.h"

namespace examples {
namespace reference {

class OrderManager final {
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
  // order management
  uint32_t create_order(const char *order_template,
                        quinclas::common::TradeDirection direction,
                        double quantity, double limit_price);
  /*
  void update_order(uint32_t order_id, uint32_t gateway_order_id,
                    double remaining_quantity);
  */
  void remove_order(uint32_t order_id);
  // exposure
  double get_exposure(quinclas::common::TradeDirection direction) const;
  void update_exposure(quinclas::common::TradeDirection direction,
                       double delta_quantity);
  void compute_and_update_exposure();
  // consistency check
  void check(const quinclas::common::MessageInfo& message_info);

 private:
  const Config& _config;
  const RiskManager& _risk_manager;
  quinclas::common::Strategy::Dispatcher& _dispatcher;
  // order management
  typedef std::chrono::system_clock::time_point time_point_t;
  uint32_t _next_order_id = 0;
  std::unordered_map<uint32_t, Order> _orders;
  std::list<std::pair<time_point_t, uint32_t> > _timeout;
  double _buy_exposure = 0.0;
  double _sell_exposure = 0.0;
  // consistency check
  time_point_t _last_update_time;
};

}  // namespace reference
}  // namespace examples
