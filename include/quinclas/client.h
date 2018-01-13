/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <glog/logging.h>

#include <quinclas/tradingapi.h>
#include <quinclas/libevent.h>
#include <quinclas/codec.h>

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace quinclas {
namespace client {

// Controller
template <typename T>
class Controller final {
  typedef std::unordered_map<std::string, std::string> gateways_t;

 public:
  explicit Controller(const gateways_t&& gateways) : _gateways(std::move(gateways)) {}
  template <typename... Args>
  void create_and_dispatch(Args&&... args) {
    const char *trace_source = "";
    Dispatcher(_gateways, std::forward<Args>(args)...).dispatch();
  }

 private:
  // Statistics
  struct Statistics final {
    uint64_t messages_sent = 0;
    uint64_t messages_received = 0;
    uint64_t connections_succeeded = 0;
    uint64_t connections_failed = 0;
  };  // Statistics

 private:
  // Dispatcher
  class Dispatcher final
      : public common::Strategy::Dispatcher {
    // Gateway
    class Gateway final
        : public common::Client,
          public common::Strategy::Dispatcher {
     public:
      Gateway(const std::string& name, const int domain, const std::string& address,
              common::Strategy& strategy, libevent::Base& base, Statistics& statistics,
              std::unordered_set<Gateway *>& callbacks, const common::MessageInfo *&trace)
          : _name(name), _domain(domain), _address(address), _base(base),
            _statistics(statistics), _event_dispatcher(*this, strategy, trace),
            _callbacks(callbacks), _state(Disconnected), _retries(0), _retry_timer(0),
            _trace(trace) {}
      bool ready() const {
        return _state == Connected;
      }
      bool refresh() {
        assert(_retry_timer >= 0);
        if (_retry_timer > 0 && 0 != --_retry_timer)
          return false;
        switch (_state) {
          case Disconnected:
            return try_connect();
          case Failed:
            return reset();
          default:
            assert(false);  // should never get here...
        }
      }
      // TODO(thraneh): handshake should be private
      void send(const common::HandshakeRequest& request) {
        send_helper(request);
      }
      // TODO(thraneh): heartbeat should be private
      void send(const common::HeartbeatRequest& request) {
        send_helper(request);
      }
      void send(const char *gateway, const common::CreateOrder& create_order) override {
        const common::CreateOrderRequest create_order_request = {
          .request_info = create_request_info(gateway),
          .create_order = create_order
        };
        send_helper(create_order_request);
      }
      void send(const char *gateway, const common::ModifyOrder& modify_order) override {
        const common::ModifyOrderRequest modify_order_request = {
          .request_info = create_request_info(gateway),
          .modify_order = modify_order
        };
        send_helper(modify_order_request);
      }
      void send(const char *gateway, const common::CancelOrder& cancel_order) override {
        const common::CancelOrderRequest cancel_order_request = {
          .request_info = create_request_info(gateway),
          .cancel_order = cancel_order
        };
        send_helper(cancel_order_request);
      }

     private:
      common::RequestInfo create_request_info(const char *gateway) {
        assert(gateway != nullptr);
        return common::RequestInfo {
          .destination = gateway,
          .trace_source = _trace ? _trace->gateway : "",
          .trace_message_id = _trace ? _trace->message_id : 0,
          .send_time = std::chrono::time_point_cast<common::duration_t>(std::chrono::system_clock::now()),
        };
      }

     private:
      bool try_connect() {
        LOG(INFO) << "[" << _name << "] connecting to " << _address.to_string();
        increment_retries();
        try {
          connect();
          _state = Connecting;
          return true;  // remove
        } catch (std::runtime_error& e) {  // TODO(thraneh): maybe a more specific exception type?
          LOG(WARNING) << "caught exception, what=\"" << e.what() << "\"";
          reset_retry_timer();
          return false;
        }
        assert(false);
      }
      bool reset() {
        _state = Disconnected;
        reset_buffers();
        reset_retry_timer();
        schedule_async_callback();
        return false;
      }
      void connection_succeeded() {
        LOG(INFO) << "[" << _name << "] connected";
        _state = Connected;
        reset_retries();
        // TODO(thraneh): notify strategy
        ++_statistics.connections_succeeded;
      }
      void connection_failed() {
        if (_state == Connected) {
          LOG(INFO) << "[" << _name << "] disconnected";
          // TODO(thraneh): notify strategy
        } else {
          LOG(INFO) << "[" << _name << "] connection attempt " << _retries << " failed";
        }
        _state = Failed;
        schedule_async_callback();
        ++_statistics.connections_failed;
      }
      void write_failed() {
        LOG(INFO) << "[" << _name << "] write failed";
        _state = Failed;
        schedule_async_callback();
      }

     private:
      void reset_retries() {
        _retries = 0;
      }
      void increment_retries() {
        ++_retries;
      }
      void reset_retry_timer() {
        const int delay[8] = { 1, 1, 1, 2, 2, 5, 5, 10 };
        _retry_timer = delay[std::min(_retries, static_cast<int>((sizeof(delay) / sizeof(delay[0])) - 1))];
      }
      void reset_buffers() {
        _buffer_event.release();
        _buffer.drain(_buffer.length());
      }
      void schedule_async_callback() {
        _callbacks.insert(this);
      }
      void connect() {
        net::Socket socket(_domain, SOCK_STREAM, 0);
        socket.non_blocking(true);
        assert(!_buffer_event);  // should have been cleared when connection attempt failed
        auto buffer_event = std::unique_ptr<libevent::BufferEvent>(
          new libevent::BufferEvent(_base, std::move(socket), 0));
        buffer_event->setcb([this](){ on_read(); },
                            [this](int what){ on_error(what); });
        buffer_event->enable(EV_READ);
        buffer_event->connect(_address);
        _buffer_event = std::move(buffer_event);
      }
      void on_error(int what) {
        if (what & BEV_EVENT_CONNECTED)
          connection_succeeded();
        else
          connection_failed();
      }
      void on_read() {
        _buffer_event->read(_buffer);
        while (true) {
          const auto envelope = _buffer.pullup(common::Envelope::LENGTH);
          if (envelope == nullptr)
            break;
          const auto length_payload = common::Envelope::decode(envelope);
          const auto bytes = common::Envelope::LENGTH + length_payload;
          const auto frame = _buffer.pullup(bytes);
          if (frame == nullptr)
            break;
          const auto payload = frame + common::Envelope::LENGTH;
          // TODO(thraneh): here we must capture MessageInfo
          _event_dispatcher.dispatch_event(payload, length_payload);
          _buffer.drain(bytes);
          ++_statistics.messages_received;
        }
      }
      template <typename R>
      void send_helper(R request) {
        if (_state != Connected) {
          LOG(ERROR) << "[" << _name << "] not connected -- unable to send the request";
          throw std::runtime_error("unable to send the request");
        }
        _flat_buffer_builder.Clear();
        _flat_buffer_builder.Finish(common::convert(_flat_buffer_builder, request));
        const auto payload = _flat_buffer_builder.GetBufferPointer();
        const auto length_payload = _flat_buffer_builder.GetSize();
        common::Envelope::encode(_envelope, length_payload);
        _buffer.add(_envelope, sizeof(_envelope));
        _buffer.add(payload, length_payload);
        try {
          _buffer_event->write(_buffer);
          ++_statistics.messages_sent;
        } catch (std::runtime_error& e) {  // TODO(thraneh): maybe a more specific exception type?
          LOG(WARNING) << "caught exception, what=\"" << e.what() << "\"";
          write_failed();
          ++_statistics.connections_failed;
          LOG(WARNING) << "[" << _name << "] failed write attempt -- unable to send the request";
          throw std::runtime_error("unable to send the request");
        }
      }

     protected:
      void on(const common::HandshakeAckEvent& event) override {
        // LOG(INFO) << "got handshake ack";
      }
      void on(const common::HeartbeatAckEvent& event) override {
        // LOG(INFO) << "got heartbeat ack";
      }

     private:
      Gateway() = delete;
      Gateway(const Gateway&) = delete;
      Gateway& operator=(const Gateway&) = delete;

     private:
      const std::string _name;
      const int _domain;
      const net::Address _address;
      libevent::Base& _base;
      Statistics& _statistics;
      common::EventDispatcher _event_dispatcher;
      std::unique_ptr<libevent::BufferEvent> _buffer_event;
      libevent::Buffer _buffer;
      flatbuffers::FlatBufferBuilder _flat_buffer_builder;
      uint8_t _envelope[common::Envelope::LENGTH];
      std::unordered_set<Gateway *>& _callbacks;
      enum { Disconnected, Connecting, Connected, Failed } _state;
      int _retries;
      int _retry_timer;
      const common::MessageInfo *&_trace;
    };  // Gateway

   public:
    template <typename... Args>
    explicit Dispatcher(const gateways_t& gateways, Args&&... args)
        : _strategy(*this, std::forward<Args>(args)...),  // request handler, then whatever the strategy needs
          _timer(_base, [this](){ on_timer(); }),
          _next_refresh(std::chrono::system_clock::now() + std::chrono::seconds(5)),
          _next_statistics(_next_refresh) {
      for (const auto iter : gateways) {
        _gateways.emplace_back(iter.first, PF_LOCAL, iter.second, _strategy, _base, _statistics, _callbacks, _trace);
        Gateway& gateway = _gateways.back();
        _gateways_by_name[iter.first] = &gateway;
        _callbacks.insert(&gateway);
      }
    }
    void dispatch() {
      _timer.add(std::chrono::seconds(1));
      _base.loop(EVLOOP_NO_EXIT_ON_EMPTY);
    }

   private:
    void send(const char *gateway, const common::CreateOrder& create_order) override {
      _gateways_by_name[gateway]->send(gateway, create_order);
    }
    void send(const char *gateway, const common::ModifyOrder& modify_order) override {
      _gateways_by_name[gateway]->send(gateway, modify_order);
    }
    void send(const char *gateway, const common::CancelOrder& cancel_order) override {
      _gateways_by_name[gateway]->send(gateway, cancel_order);
    }
    void on_timer() {
      auto now = std::chrono::system_clock::now();
      if (refresh(now)) {
        remove_zombie_connections();
        send_heartbeat(now);
      }
      if (statistics(now)) {
        write_statistics();
      }
    }
    bool refresh(const std::chrono::system_clock::time_point now) {
      if (now < _next_refresh)
        return false;
      while (true) {
        _next_refresh += std::chrono::seconds(5);
        if (now < _next_refresh)
          return true;
      }
    }
    void remove_zombie_connections() {
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
    void send_heartbeat(const std::chrono::system_clock::time_point now) {
      const common::RequestInfo request_info = {
        .destination = "",
        .trace_source = "",
      };
      const common::Heartbeat heartbeat = {
        .opaque = std::chrono::time_point_cast<common::duration_t>(now),
      };
      const common::HeartbeatRequest request = {
        .request_info = request_info,
        .heartbeat = heartbeat,
      };
      for (auto& iter : _gateways)
        if (iter.ready())
          iter.send(request);
    }
    bool statistics(const std::chrono::system_clock::time_point now) {
      if (now < _next_statistics)
        return false;
      while (true) {
        _next_statistics += std::chrono::minutes(5);
        if (now < _next_statistics)
          return true;
      }
    }
    void write_statistics() {
      std::cout << std::flush;
      LOG(INFO) << "Statistics("
        "messages_sent=" << _statistics.messages_sent << ", "
        "messages_received=" << _statistics.messages_received << ", "
        "connections_succeeded=" << _statistics.connections_succeeded << ", "
        "connections_failed=" << _statistics.connections_failed <<
        ")";
      google::FlushLogFiles(google::GLOG_INFO);
    }

   private:
    Dispatcher() = delete;
    Dispatcher(const Dispatcher&) = delete;
    Dispatcher& operator=(const Dispatcher&) = delete;

   private:
    T _strategy;
    libevent::Base _base;
    libevent::Timer _timer;
    Statistics _statistics;
    std::list<Gateway> _gateways;
    std::unordered_map<std::string, Gateway *> _gateways_by_name;
    std::unordered_set<Gateway *> _callbacks;
    const common::MessageInfo *_trace = nullptr;
    std::chrono::system_clock::time_point _next_refresh;
    std::chrono::system_clock::time_point _next_statistics;
  };  // Dispatcher

 private:
  Controller() = delete;
  Controller(const Controller&) = delete;
  Controller& operator=(const Controller&) = delete;

 private:
  gateways_t _gateways;
};  // Controller

}  // namespace client
}  // namespace quinclas
