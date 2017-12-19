/* Copyright (c) 2017,  Hans Erik Thrane */

#pragma once

#include <glog/logging.h>

#include <quinclas/tradingapi.h>
#include <quinclas/io/libevent.h>

#include <algorithm>
#include <iomanip>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace examples {
namespace trading_engine {

template <typename T>
class Controller final {
  typedef std::unordered_map<std::string, std::string> gateways_t;

 public:
  explicit Controller(const gateways_t&& gateways) : _gateways(std::move(gateways)) {}
  template <typename... Args>
  void create_and_dispatch(Args&&... args) {
    Dispatcher(_gateways, std::forward<Args>(args)...).dispatch();
  }

 private:
  class Dispatcher final
      : public quinclas::common::Strategy::Dispatcher,
        public quinclas::io::libevent::TimerEvent::Handler {
    // maintains gateway state
    class Gateway final {
     public:
      Gateway(const std::string& name, const int domain, const std::string& address,
              quinclas::common::Strategy& strategy, quinclas::io::libevent::Base& base,
              std::unordered_set<Gateway *>& callbacks)
          : _name(name), _domain(domain), _address(address), _strategy(strategy), _base(base), _callbacks(callbacks),
            _state(Disconnected), _retry(0), _countdown(0) {}
      bool refresh() {
        if (_countdown > 0 && 0 != --_countdown)
          return false;
        switch (_state) {
          case Disconnected: {
            LOG(INFO) << "gateway name=" << _name << ", state=connecting, retry=" << _retry;
            ++_retry;
            try {
              connect();
              _state = Connecting;
              return true;  // remove
            } catch (std::runtime_error& e) {
              LOG(WARNING) << e.what();
              _countdown = std::min(std::max(_retry, 1), 10);  // TODO(thraneh): table lookup
              return false;  // keep
            }
          }
          case Failed: {
            LOG(INFO) << "gateway name=" << _name << ", state=disconnected";
            _buffer_event.release();
            _state = Disconnected;
            _countdown = std::min(std::max(_retry, 1), 10);  // TODO(thraneh): table lookup
            _callbacks.insert(this);
            return false;  // keep
          }
          default:
            assert(false);  // should never get here...
        }
      }

     private:
      void connect() {
        quinclas::io::net::Socket socket(_domain, SOCK_STREAM, 0);
        socket.non_blocking(true);
        auto buffer_event = std::unique_ptr<quinclas::io::libevent::BufferEvent>(
          new quinclas::io::libevent::BufferEvent(_base, std::move(socket)));
        buffer_event->setcb(on_read, nullptr, on_error, this);
        buffer_event->enable(EV_READ);
        buffer_event->connect(_address);
        _buffer_event = std::move(buffer_event);
      }
      static void on_read(struct bufferevent *bev, void *arg) {
        LOG(INFO) << "read";
        // TODO(thraneh): decode and notify strategy
      }
      static void on_error(struct bufferevent *bev, short what, void *arg) {  // NOLINT short
        auto& self = *reinterpret_cast<Gateway *>(arg);
        LOG(INFO) << "gateway name=" << self._name << ", error=0x" << std::setw(2) << std::hex << what;
        if (what & BEV_EVENT_CONNECTED) {
          LOG(INFO) << "gateway name=" << self._name << ", state=connected";
          self._state = Connected;
          self._retry = 0;
        } else {  // this should catch all real errors
          LOG(INFO) << "gateway name=" << self._name << ", state=failed";
          self._state = Failed;
          self._callbacks.insert(&self);
        }
      }

     private:
      std::string _name;
      int _domain;
      quinclas::io::net::Address _address;
      quinclas::common::Strategy& _strategy;
      quinclas::io::libevent::Base& _base;
      std::unordered_set<Gateway *>& _callbacks;
      enum { Disconnected, Connecting, Connected, Failed } _state;
      int _retry;
      int _countdown;
      std::unique_ptr<quinclas::io::libevent::BufferEvent> _buffer_event;
    };

   public:
    template <typename... Args>
    explicit Dispatcher(const gateways_t& gateways, Args&&... args)
        : _strategy(*this, std::forward<Args>(args)...),  // request handler, then whatever the strategy needs
          _timer(*this, _base) {
      for (const auto iter : gateways) {
        _gateways.emplace_back(Gateway(iter.first, PF_LOCAL, iter.second, _strategy, _base, _callbacks));
        Gateway *gateway = &(*_gateways.rbegin());
        _gateways_by_name[iter.first] = gateway;
        _callbacks.insert(gateway);
      }
    }
    void dispatch() {
      _timer.add({.tv_sec = 1});
      _base.loop(EVLOOP_NO_EXIT_ON_EMPTY);
    }

   private:
    void send(const quinclas::common::CreateOrderRequest& create_order_request) override {
      Gateway *gateway = _gateways_by_name[create_order_request.request_info.destination];
      // TODO(thraneh): implement
    }
    void send(const quinclas::common::ModifyOrderRequest& modify_order_request) override {
      Gateway *gateway = _gateways_by_name[modify_order_request.request_info.destination];
      // TODO(thraneh): implement
    }
    void send(const quinclas::common::CancelOrderRequest& cancel_order_request) override {
      Gateway *gateway = _gateways_by_name[cancel_order_request.request_info.destination];
      // TODO(thraneh): implement
    }
    void on_timer() override {
      std::list<Gateway *> remove;
      for (const auto iter : _callbacks)
        if ((*iter).refresh())
          remove.push_back(iter);
      if (_callbacks.size() == remove.size()) {
        _callbacks.clear();
      } else {
        for (auto iter : remove)
          _callbacks.erase(iter);
      }
    }

   private:
    T _strategy;
    quinclas::io::libevent::Base _base;
    quinclas::io::libevent::TimerEvent _timer;
    std::vector<Gateway> _gateways;
    std::unordered_map<std::string, Gateway *> _gateways_by_name;
    std::unordered_set<Gateway *> _callbacks;
  };

 private:
  gateways_t _gateways;
};

}  // namespace trading_engine
}  // namespace examples
