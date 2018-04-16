/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <chrono>
#include <list>
#include <stdexcept>
#include <string>
#include <utility>

namespace roq {
namespace simulation {

// Generator
class Generator {
 public:
  Generator() {}
  virtual ~Generator() {}
  virtual std::pair<bool, std::chrono::system_clock::time_point> fetch() = 0;
  virtual void dispatch(Strategy& strategy) = 0;

 private:
  Generator(Generator&) = delete;
  Generator& operator=(Generator&) = delete;
};

// Controller
template <typename T>
class Controller final {
  typedef std::list<std::unique_ptr<Generator> > generators_t;

 public:
  explicit Controller(generators_t&& generators) : _generators(std::move(generators)) {}
  template <typename... Args>
  void create_and_dispatch(Args&&... args) {
    Dispatcher(_generators, std::forward<Args>(args)...).dispatch();
  }

 private:
  // Dispatcher
  class Dispatcher final
      : public Strategy::Dispatcher {
   public:
    template <typename... Args>
    explicit Dispatcher(generators_t& generators, Args&&... args)
        : _generators(generators),
          _strategy(*this, std::forward<Args>(args)...) {}
    void dispatch() {
      initialize();
      // TODO(thraneh): interleave multiple generators
      for (auto& iter : _generators) {
        while (true) {
          auto res = iter->fetch();
          if (res.first) {
            iter->dispatch(_strategy);
          } else {
            return;
          }
        }
      }
    }

   private:
    void initialize() {
      auto& strategy = static_cast<Strategy&>(_strategy);
      const auto& subscriptions = strategy.get_subscriptions();
      for (const auto& iter_1 : subscriptions) {
        const auto& gateway = iter_1.first.c_str();
        const auto& tmp_1 = iter_1.second;
        download_begin_event(gateway);
        gateway_status(gateway, "MDUser");  // FIXME(thraneh): clean up!
        gateway_status(gateway, "Trader");  // FIXME(thraneh): clean up!
        for (const auto& iter_2 : tmp_1) {
          const auto& exchange = iter_2.first.c_str();
          const auto& tmp_2 = iter_2.second;
          for (const auto& iter_3 : tmp_2) {
            auto symbol = iter_3.c_str();
            market_status(gateway, exchange, symbol);
          }
        }
        download_end_event(gateway);
      }
    }
    void download_begin_event(const char *gateway) {
      auto& strategy = static_cast<Strategy&>(_strategy);
      MessageInfo message_info { .source = gateway };
      DownloadBegin download_begin {};
      strategy.on(DownloadBeginEvent {
          .message_info = message_info,
          .download_begin = download_begin
          });
    }
    void download_end_event(const char *gateway) {
      auto& strategy = static_cast<Strategy&>(_strategy);
      MessageInfo message_info { .source = gateway };
      DownloadEnd download_end {};
      strategy.on(DownloadEndEvent {
          .message_info = message_info,
          .download_end = download_end
          });
    }
    void gateway_status(const char *gateway, const char *name) {
      auto& strategy = static_cast<Strategy&>(_strategy);
      MessageInfo message_info { .source = gateway };
      GatewayStatus gateway_status {
        .name = name,
        .status = GatewayState::Ready,
      };
      strategy.on(GatewayStatusEvent {
          .message_info = message_info,
          .gateway_status = gateway_status
          });
    }
    void market_status(
        const char *gateway,
        const char *exchange,
        const char *symbol) {
      auto& strategy = static_cast<Strategy&>(_strategy);
      MessageInfo message_info { .source = gateway };
      MarketStatus market_status {
        .exchange = exchange,
        .symbol = symbol,
        .trading_status = TradingStatus::Open,
      };
      strategy.on(MarketStatusEvent {
          .message_info = message_info,
          .market_status = market_status
          });
    }

   private:
    void send(const CreateOrder& create_order, const std::string& gateway) override {
      // FIXME(thraneh): validate + enqueue
      create_order_ack(create_order, gateway.c_str());
      order_update(create_order, gateway.c_str());
      trade_update(create_order, gateway.c_str());
    }
    void send(const ModifyOrder& modify_order, const std::string& gateway) override {
      // FIXME(thraneh): validate + enqueue
      modify_order_ack(modify_order, gateway.c_str());
    }
    void send(const CancelOrder& cancel_order, const std::string& gateway) override {
      // FIXME(thraneh): validate + enqueue
      cancel_order_ack(cancel_order, gateway.c_str());
    }
    void create_order_ack(const CreateOrder& create_order, const char *gateway) {
      auto& strategy = static_cast<Strategy&>(_strategy);
      MessageInfo message_info { .source = gateway };
      CreateOrderAck create_order_ack {
        .order_id = create_order.order_id,
        .failure = false,
        .reason = "",
        .order_local_id = 0,
        .order_external_id = "",
      };
      strategy.on(CreateOrderAckEvent {
          .message_info = message_info,
          .create_order_ack = create_order_ack
          });
    }
    void modify_order_ack(const ModifyOrder& modify_order, const char *gateway) {
      auto& strategy = static_cast<Strategy&>(_strategy);
      MessageInfo message_info { .source = gateway };
      ModifyOrderAck modify_order_ack {
        .order_id = modify_order.order_id,
        .failure = true,
        .reason = "NOT_SUPPORTED",
        .order_local_id = 0,
        .order_external_id = "",
      };
      strategy.on(ModifyOrderAckEvent {
          .message_info = message_info,
          .modify_order_ack = modify_order_ack
          });
    }
    void cancel_order_ack(const CancelOrder& cancel_order, const char *gateway) {
      auto& strategy = static_cast<Strategy&>(_strategy);
      MessageInfo message_info { .source = gateway };
      CancelOrderAck cancel_order_ack {
        .order_id = cancel_order.order_id,
        .failure = true,
        .reason = "NOT_SUPPORTED",
        .order_local_id = 0,
        .order_external_id = "",
      };
      strategy.on(CancelOrderAckEvent {
          .message_info = message_info,
          .cancel_order_ack = cancel_order_ack
          });
    }
    void order_update(const CreateOrder& create_order, const char *gateway) {
      auto& strategy = static_cast<Strategy&>(_strategy);
      MessageInfo message_info { .source = gateway };
      OrderUpdate order_update {
        .order_id = create_order.order_id,
        .account = create_order.account,
        .exchange = create_order.exchange,
        .symbol = create_order.symbol,
        .order_status = OrderStatus::Completed,
        .side = create_order.side,
        .remaining_quantity = 0.0,
        .traded_quantity = create_order.quantity,
        .order_template = create_order.order_template,
        .insert_time = std::chrono::time_point_cast<duration_t>(
            std::chrono::system_clock::time_point()),
        .cancel_time = std::chrono::time_point_cast<duration_t>(
            std::chrono::system_clock::time_point()),
        .order_local_id = 0,
        .order_external_id = "",
      };
      strategy.on(OrderUpdateEvent {
          .message_info = message_info,
          .order_update = order_update
          });
    }
    void trade_update(const CreateOrder& create_order, const char *gateway) {
      auto& strategy = static_cast<Strategy&>(_strategy);
      MessageInfo message_info { .source = gateway };
      TradeUpdate trade_update {
        .trade_id = create_order.order_id,
        .order_id = create_order.order_id,
        .account = create_order.account,
        .exchange = create_order.exchange,
        .symbol = create_order.symbol,
        .side = create_order.side,
        .quantity = 0.0,
        .price = create_order.limit_price,
        .order_template = create_order.order_template,
        .trade_time = std::chrono::time_point_cast<duration_t>(
            std::chrono::system_clock::time_point()),
        .order_local_id = 0,
        .order_external_id = "",
        .trade_external_id = "",
      };
      strategy.on(TradeUpdateEvent {
          .message_info = message_info,
          .trade_update = trade_update
          });
    }

   private:
    Dispatcher() = delete;
    Dispatcher(const Dispatcher&) = delete;
    Dispatcher& operator=(const Dispatcher&) = delete;

   private:
    T _strategy;
    generators_t& _generators;
  };  // Dispatcher

 private:
  Controller() = delete;
  Controller(const Controller&) = delete;
  Controller& operator=(const Controller&) = delete;

 private:
  generators_t _generators;
};  // Controller

}  // namespace simulation
}  // namespace roq
