/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/api.h>

#include <chrono>
#include <list>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

namespace roq {
namespace simulation {

// Generator
class Generator {
 public:
  class Dispatcher {
   public:
    virtual void on(const BatchBeginEvent& event) = 0;
    virtual void on(const BatchEndEvent& event) = 0;
    virtual void on(const SessionStatisticsEvent& event) = 0;
    virtual void on(const DailyStatisticsEvent& event) = 0;
    virtual void on(const MarketByPriceEvent& event) = 0;
    virtual void on(const TradeSummaryEvent& event) = 0;
  };
  Generator() {}
  virtual ~Generator() {}
  virtual std::pair<bool, std::chrono::system_clock::time_point> fetch() = 0;
  virtual void dispatch(Dispatcher& dispatcher) = 0;

 private:
  Generator(const Generator&) = delete;
  void operator=(const Generator&) = delete;
};

// Matcher
class Matcher {
 public:
  class Dispatcher {
   public:
    virtual void on(const CreateOrderAckEvent&) = 0;
    virtual void on(const ModifyOrderAckEvent&) = 0;
    virtual void on(const CancelOrderAckEvent&) = 0;
    virtual void on(const OrderUpdateEvent&) = 0;
    virtual void on(const TradeUpdateEvent&) = 0;
    virtual void on(const PositionUpdateEvent&) = 0;
  };
  Matcher(Dispatcher& dispatcher, const std::string& name)
      : _dispatcher(dispatcher),
        _name(name) {
  }
  virtual ~Matcher() {}
  // TODO(thraneh): add timer + next_update
  virtual void on(const BatchBeginEvent& event) = 0;
  virtual void on(const BatchEndEvent& event) = 0;
  virtual void on(const SessionStatisticsEvent& event) = 0;
  virtual void on(const DailyStatisticsEvent& event) = 0;
  virtual void on(const MarketByPriceEvent& event) = 0;
  virtual void on(const TradeSummaryEvent& event) = 0;
  virtual void on(const CreateOrder& create_order) = 0;
  virtual void on(const ModifyOrder& modify_order) = 0;
  virtual void on(const CancelOrder& cancel_order) = 0;

 private:
  Matcher(const Matcher&) = delete;
  void operator=(const Matcher&) = delete;

 protected:
  Dispatcher& _dispatcher;
  const std::string _name;

 protected:
  void send(const CreateOrderAck& create_order_ack) {
    auto message_info = MessageInfo {
      .source = _name.c_str()
    };
    auto event = CreateOrderAckEvent {
      .message_info = message_info,
      .create_order_ack = create_order_ack
    };
    _dispatcher.on(event);
  }
  void send(const ModifyOrderAck& modify_order_ack) {
    auto message_info = MessageInfo {
      .source = _name.c_str()
    };
    auto event = ModifyOrderAckEvent {
      .message_info = message_info,
      .modify_order_ack = modify_order_ack
    };
    _dispatcher.on(event);
  }
  void send(const CancelOrderAck& cancel_order_ack) {
    auto message_info = MessageInfo {
      .source = _name.c_str()
    };
    auto event = CancelOrderAckEvent {
      .message_info = message_info,
      .cancel_order_ack = cancel_order_ack
    };
    _dispatcher.on(event);
  }
  void send(const OrderUpdate& order_update) {
    auto message_info = MessageInfo {
      .source = _name.c_str()
    };
    auto event = OrderUpdateEvent {
      .message_info = message_info,
      .order_update = order_update
    };
    _dispatcher.on(event);
  }
  void send(const TradeUpdate& trade_update) {
    auto message_info = MessageInfo {
      .source = _name.c_str()
    };
    auto event = TradeUpdateEvent {
      .message_info = message_info,
      .trade_update = trade_update
    };
    _dispatcher.on(event);
  }
  void send(const PositionUpdate& position_update) {
    auto message_info = MessageInfo {
      .source = _name.c_str()
    };
    auto event = PositionUpdateEvent {
      .message_info = message_info,
      .position_update = position_update
    };
    _dispatcher.on(event);
  }
};

// NoMatcher
class NoMatcher final : public Matcher {
 public:
  NoMatcher(
      Matcher::Dispatcher& dispatcher,
      const std::string& name)
      : Matcher(dispatcher, name) {
  }
  void on(const BatchBeginEvent& event) override {
  }
  void on(const BatchEndEvent& event) override {
  }
  void on(const SessionStatisticsEvent& event) override {
  }
  void on(const DailyStatisticsEvent& event) override {
  }
  void on(const MarketByPriceEvent& event) override {
  }
  void on(const TradeSummaryEvent& event) override {
  }
  void on(const CreateOrder& create_order) override {
    throw std::runtime_error("Unexpected");
  }
  void on(const ModifyOrder& modify_order) override {
    throw std::runtime_error("Unexpected");
  }
  void on(const CancelOrder& cancel_order) override {
    throw std::runtime_error("Unexpected");
  }
};

// SimpleMatcher
class SimpleMatcher final : public Matcher {
 public:
  SimpleMatcher(
      Matcher::Dispatcher& dispatcher,
      const std::string& name)
      : Matcher(dispatcher, name) {
    if (name.empty())
      throw std::runtime_error("Gateway name is required");
  }
  void on(const BatchBeginEvent& event) override {
  }
  void on(const BatchEndEvent& event) override {
  }
  void on(const SessionStatisticsEvent& event) override {
  }
  void on(const DailyStatisticsEvent& event) override {
  }
  void on(const MarketByPriceEvent& event) override {
  }
  void on(const TradeSummaryEvent& event) override {
  }
  void on(const CreateOrder& create_order) override {
    auto account = create_order.account;
    auto order_id = create_order.order_id;
    switch (create_order.side) {
      case Side::Buy:
      case Side::Sell:
        break;
      default: {
        auto create_order_ack = CreateOrderAck {
          .account = account,
          .order_id = order_id,
          .failure = true,
          .reason = "Invalid side",
          .order_local_id = 0,
          .order_external_id = "",
        };
        send(create_order_ack);
        return;
      }
    }
    auto exchange = create_order.exchange;
    auto symbol = create_order.symbol;
    auto side = create_order.side;
    auto quantity = create_order.quantity;
    auto create_order_ack = CreateOrderAck {
      .account = account,
      .order_id = order_id,
      .failure = false,
      .reason = "",
      .order_local_id = 0,
      .order_external_id = "",
    };
    send(create_order_ack);
    auto order_update = OrderUpdate {
      .account = account,
      .order_id = order_id,
      .exchange = exchange,
      .symbol = symbol,
      .order_status = OrderStatus::Completed,
      .side = side,
      .remaining_quantity = 0.0,
      .traded_quantity = quantity,
      .position_effect = create_order.position_effect,
      .order_template = create_order.order_template,
      .insert_time = std::chrono::time_point_cast<duration_t>(
          std::chrono::system_clock::time_point()),
      .cancel_time = std::chrono::time_point_cast<duration_t>(
          std::chrono::system_clock::time_point()),
      .order_local_id = 0,
      .order_external_id = "",
    };
    send(order_update);
    auto trade_update = TradeUpdate {
      .account = account,
      .trade_id = order_id,
      .order_id = order_id,
      .exchange = exchange,
      .symbol = symbol,
      .side = side,
      .quantity = 0.0,
      .price = create_order.limit_price,
      .position_effect = create_order.position_effect,
      .order_template = create_order.order_template,
      .trade_time = std::chrono::time_point_cast<duration_t>(
          std::chrono::system_clock::time_point()),
      .order_external_id = "",
      .trade_external_id = "",
    };
    send(trade_update);
    auto& position = _positions[account][exchange][symbol][side];
    position += quantity;
    auto position_update = PositionUpdate {
        .account = account,
        .exchange = exchange,
        .symbol = symbol,
        .side = side,
        .last_trade_id = order_id,
        .position = position,
        .position_cost = 0.0,
        .position_yesterday = 0.0,
        .position_cost_yesterday = 0.0,
    };
    send(position_update);
  }
  void on(const ModifyOrder& modify_order) override {
    auto modify_order_ack = ModifyOrderAck {
      .account = modify_order.account,
      .order_id = modify_order.order_id,
      .failure = true,
      .reason = "NOT_SUPPORTED",
      .order_local_id = 0,
      .order_external_id = "",
    };
    send(modify_order_ack);
  }
  void on(const CancelOrder& cancel_order) override {
    auto cancel_order_ack = CancelOrderAck {
      .account = cancel_order.account,
      .order_id = cancel_order.order_id,
      .failure = true,
      .reason = "NOT_SUPPORTED",
      .order_local_id = 0,
      .order_external_id = "",
    };
    send(cancel_order_ack);
  }

 private:
  // TODO(thraneh): drop this work-around when moving to C++17
  // SO18837857
  struct EnumHash {
    template <typename T>
    std::size_t operator()(T t) const {
      return static_cast<std::size_t>(t);
    }
  };
  std::unordered_map<std::string,                   // account
    std::unordered_map<std::string,                 // exchange
      std::unordered_map<std::string,               // symbol
        std::unordered_map<Side, double, EnumHash> > > > _positions;
};

// Controller
template <typename T, typename M>
class Controller final {
  typedef std::list<std::unique_ptr<Generator> > generators_t;

 public:
  explicit Controller(
      generators_t&& generators,
      const std::string& gateway = "")
      : _generators(std::move(generators)),
        _gateway(gateway) {
  }
  template <typename... Args>
  void create_and_dispatch(Args&&... args) {
    Dispatcher(
      _gateway,
      _generators,
      std::forward<Args>(args)...).dispatch();
  }

 private:
  // Dispatcher
  class Dispatcher final
      : public Client::Dispatcher,
        public Generator::Dispatcher,
        public Matcher::Dispatcher {
   public:
    template <typename... Args>
    explicit Dispatcher(
        const std::string& name,
        generators_t& generators,
        Args&&... args)
        : _name(name),
          _generators(generators),
          _matcher(*this, name),
          _client(*this, std::forward<Args>(args)...) {}
    void dispatch() {
      initialize();
      // TODO(thraneh): interleave multiple generators
      for (auto& iter : _generators) {
        while (true) {
          auto res = iter->fetch();
          if (res.first) {
            generate_timer_events(res.second);
            iter->dispatch(*this);
          } else {
            return;  // finished
          }
        }
      }
    }

   private:
    void generate_timer_events(
        std::chrono::system_clock::time_point next) {
      if (_next_timer == std::chrono::system_clock::time_point())
        _next_timer = next;
      auto& client = static_cast<Client&>(_client);
      while (_next_timer <= next) {
        client.on(TimerEvent{});
        _next_timer += std::chrono::seconds(1);
      }
    }
    void initialize() {
      auto& client = static_cast<Client&>(_client);
      const auto& subscriptions = client.get_subscriptions();
      // download per gateway ...
      for (const auto& iter : subscriptions) {
        const auto& gateway = iter.first;
        const auto& subscription = iter.second;
        // ... reference data
        download_begin_event(gateway, "");
        for (const auto& symbols : subscription.symbols_by_exchange) {
          const auto& exchange = symbols.first;
          for (const auto& symbol : symbols.second)
            market_status(gateway, exchange, symbol);
        }
        download_end_event(gateway, "");
        // ... accounts (order manager)
        for (const auto& account : subscription.accounts) {
          order_manager_status(gateway, account);
          for (const auto& symbols : subscription.symbols_by_exchange) {
            const auto& exchange = symbols.first;
            for (const auto& symbol : symbols.second) {
              position_update(
                  gateway,
                  account,
                  exchange,
                  symbol,
                  Side::Buy);
              position_update(
                  gateway,
                  account,
                  exchange,
                  symbol,
                  Side::Sell);
            }
          }
        }
        // ... market data
        market_data_status(gateway);
      }
    }
    void download_begin_event(
        const std::string& gateway,
        const std::string& account) {
      auto message_info = MessageInfo {
        .source = gateway.c_str(),
      };
      auto download_begin = DownloadBegin {
        .account = account.c_str(),
      };
      auto event = DownloadBeginEvent {
        .message_info = message_info,
        .download_begin = download_begin
      };
      static_cast<Client&>(_client).on(event);
    }
    void download_end_event(
        const std::string& gateway,
        const std::string& account) {
      auto message_info = MessageInfo {
        .source = gateway.c_str(),
      };
      auto download_end = DownloadEnd {
        .account = account.c_str(),
      };
      auto event = DownloadEndEvent {
        .message_info = message_info,
        .download_end = download_end
      };
      static_cast<Client&>(_client).on(event);
    }
    void market_data_status(
        const std::string& gateway) {
      auto message_info = MessageInfo {
        .source = gateway.c_str(),
      };
      auto market_data_status = MarketDataStatus {
        .status = GatewayStatus::Ready,
      };
      auto event = MarketDataStatusEvent {
        .message_info = message_info,
        .market_data_status = market_data_status
      };
      static_cast<Client&>(_client).on(event);
    }
    void order_manager_status(
        const std::string& gateway,
        const std::string& account) {
      auto message_info = MessageInfo {
        .source = gateway.c_str(),
      };
      auto order_manager_status = OrderManagerStatus {
        .account = account.c_str(),
        .status = GatewayStatus::Ready,
      };
      auto event = OrderManagerStatusEvent {
        .message_info = message_info,
        .order_manager_status = order_manager_status
      };
      static_cast<Client&>(_client).on(event);
    }
    void market_status(
        const std::string& gateway,
        const std::string& exchange,
        const std::string& symbol) {
      auto message_info = MessageInfo {
        .source = gateway.c_str(),
      };
      auto market_status = MarketStatus {
        .exchange = exchange.c_str(),
        .symbol = symbol.c_str(),
        .trading_status = TradingStatus::Open,
      };
      auto event = MarketStatusEvent {
        .message_info = message_info,
        .market_status = market_status
      };
      static_cast<Client&>(_client).on(event);
    }
    // TODO(thraneh): matcher should probably own all position updates
    void position_update(
        const std::string& gateway,
        const std::string& account,
        const std::string& exchange,
        const std::string& symbol,
        Side side) {
      auto message_info = MessageInfo {
        .source = gateway.c_str(),
      };
      auto position_update = PositionUpdate {
        .account = account.c_str(),
        .exchange = exchange.c_str(),
        .symbol = symbol.c_str(),
        .side = side,
        .last_trade_id = 0,
        .position = 0.0,
        .position_cost = 0.0,
        .position_yesterday = 0.0,
        .position_cost_yesterday = 0.0,
      };
      auto event = PositionUpdateEvent {
        .message_info = message_info,
        .position_update = position_update
      };
      static_cast<Client&>(_client).on(event);
    }

   private:
    void on(const BatchBeginEvent& event) override {
      _matcher.on(event);
      static_cast<Client&>(_client).on(event);
    }
    void on(const BatchEndEvent& event) override {
      _matcher.on(event);
      static_cast<Client&>(_client).on(event);
    }
    void on(const SessionStatisticsEvent& event) override {
      _matcher.on(event);
      static_cast<Client&>(_client).on(event);
    }
    void on(const DailyStatisticsEvent& event) override {
      _matcher.on(event);
      static_cast<Client&>(_client).on(event);
    }
    void on(const MarketByPriceEvent& event) override {
      _matcher.on(event);
      static_cast<Client&>(_client).on(event);
    }
    void on(const TradeSummaryEvent& event) override {
      _matcher.on(event);
      static_cast<Client&>(_client).on(event);
    }

   private:
    void on(const CreateOrderAckEvent& event) override {
      static_cast<Client&>(_client).on(event);
    }
    void on(const ModifyOrderAckEvent& event) override {
      static_cast<Client&>(_client).on(event);
    }
    void on(const CancelOrderAckEvent& event) override {
      static_cast<Client&>(_client).on(event);
    }
    void on(const OrderUpdateEvent& event) override {
      static_cast<Client&>(_client).on(event);
    }
    void on(const TradeUpdateEvent& event) override {
      static_cast<Client&>(_client).on(event);
    }
    void on(const PositionUpdateEvent& event) override {
      static_cast<Client&>(_client).on(event);
    }

   private:
    void send(const CreateOrder& create_order, const std::string& gateway) override {
      _matcher.on(create_order);
    }
    void send(const ModifyOrder& modify_order, const std::string& gateway) override {
      _matcher.on(modify_order);
    }
    void send(const CancelOrder& cancel_order, const std::string& gateway) override {
      _matcher.on(cancel_order);
    }

   private:
    Dispatcher() = delete;
    Dispatcher(const Dispatcher&) = delete;
    void operator=(const Dispatcher&) = delete;

   private:
    const std::string _name;
    T _client;
    generators_t& _generators;
    std::chrono::system_clock::time_point _next_timer;
    M _matcher;
  };  // Dispatcher

 private:
  Controller() = delete;
  Controller(const Controller&) = delete;
  void operator=(const Controller&) = delete;

 private:
  generators_t _generators;
  const std::string _gateway;
};  // Controller

}  // namespace simulation
}  // namespace roq
