/* Copyright (c) 2017, Hans Erik Thrane */

#pragma once

#include <glog/logging.h>

#include <quinclas/tradingapi.h>
#include <quinclas/libevent.h>
#include <quinclas/codec.h>

#include <algorithm>
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
    Dispatcher(_gateways, std::forward<Args>(args)...).dispatch();
  }

 private:
  // Dispatcher
  class Dispatcher final
      : public common::Strategy::Dispatcher,
        public libevent::TimerEvent::Handler {
    // Gateway
    class Gateway final
        : public common::Client,
          public common::Strategy::Dispatcher {
     public:
      Gateway(const std::string& name, const int domain, const std::string& address,
              common::Strategy& strategy, libevent::Base& base,
              std::unordered_set<Gateway *>& callbacks)
          : _name(name), _domain(domain), _address(address), _base(base), _event_dispatcher(*this, strategy),
            _callbacks(callbacks), _state(Disconnected), _retries(0), _retry_timer(0) {}
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
      void send(const common::HandshakeRequest& request) override {
        send_helper(request);
      }
      void send(const common::HeartbeatRequest& request) override {
        send_helper(request);
      }
      void send(const common::CreateOrderRequest& request) override {
        send_helper(request);
      }
      void send(const common::ModifyOrderRequest& request) override {
        send_helper(request);
      }
      void send(const common::CancelOrderRequest& request) override {
        send_helper(request);
      }

     private:
      bool try_connect() {
        LOG(INFO) << "gateway: " << _name << " connecting to " << _address.to_string();
        increment_retries();
        try {
          connect();
          _state = Connecting;
          return true;  // remove
        } catch (std::runtime_error& e) {  // TODO(thraneh): maybe a more specific exception type?
          LOG(WARNING) << "gateway: caught exception, what=\"" << e.what() << "\"";
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
        LOG(INFO) << "gateway: " << _name << " connected";
        _state = Connected;
        reset_retries();
        // TODO(thraneh): notify strategy
      }
      void connection_failed() {
        if (_state == Connected) {
          LOG(INFO) << "gateway: " << _name << " disconnected";
          // TODO(thraneh): notify strategy
        } else {
          LOG(INFO) << "gateway: " << _name << " connection attempt " << _retries << " failed";
        }
        _state = Failed;
        schedule_async_callback();
      }
      void write_failed() {
        LOG(INFO) << "gateway: " << _name << " write failed";
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
          new libevent::BufferEvent(_base, std::move(socket)));
        buffer_event->setcb(on_read, nullptr, on_error, this);
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
          _event_dispatcher.dispatch_event(payload, length_payload);
          _buffer.drain(bytes);
        }
      }
      template <typename R>
      void send_helper(const R& request) {
        if (_state != Connected) {
          LOG(ERROR) << "gateway: " << _name << " not connected -- unable to send the request";
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
        } catch (std::runtime_error& e) {  // TODO(thraneh): maybe a more specific exception type?
          LOG(WARNING) << "gateway: caught exception, what=\"" << e.what() << "\"";
          write_failed();
          LOG(WARNING) << "gateway: " << _name << " failed write attempt -- unable to send the request";
          throw std::runtime_error("unable to send the request");
        }
      }

     private:
      static void on_error(struct bufferevent *bev, short what, void *arg) {  // NOLINT short
        reinterpret_cast<Gateway *>(arg)->on_error(what);
      }
      static void on_read(struct bufferevent *bev, void *arg) {
        reinterpret_cast<Gateway *>(arg)->on_read();
      }

     protected:
      void on(const common::HandshakeAckEvent& event) override {
        LOG(INFO) << "got handshake ack";
      }
      void on(const common::HeartbeatAckEvent& event) override {
        LOG(INFO) << "got heartbeeat ack";
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
      common::EventDispatcher _event_dispatcher;
      std::unique_ptr<libevent::BufferEvent> _buffer_event;
      libevent::Buffer _buffer;
      flatbuffers::FlatBufferBuilder _flat_buffer_builder;
      uint8_t _envelope[common::Envelope::LENGTH];
      std::unordered_set<Gateway *>& _callbacks;
      enum { Disconnected, Connecting, Connected, Failed } _state;
      int _retries;
      int _retry_timer;
    };

   public:
    template <typename... Args>
    explicit Dispatcher(const gateways_t& gateways, Args&&... args)
        : _strategy(*this, std::forward<Args>(args)...),  // request handler, then whatever the strategy needs
          _timer(*this, _base) {
      for (const auto iter : gateways) {
        _gateways.emplace_back(iter.first, PF_LOCAL, iter.second, _strategy, _base, _callbacks);
        Gateway& gateway = _gateways.back();
        _gateways_by_name[iter.first] = &gateway;
        _callbacks.insert(&gateway);
      }
    }
    void dispatch() {
      _timer.add({.tv_sec = 1});
      _base.loop(EVLOOP_NO_EXIT_ON_EMPTY);
    }

   private:
    void send(const common::CreateOrderRequest& create_order_request) override {
      _gateways_by_name[create_order_request.request_info.destination]->send(create_order_request);
    }
    void send(const common::ModifyOrderRequest& modify_order_request) override {
      _gateways_by_name[modify_order_request.request_info.destination]->send(modify_order_request);
    }
    void send(const common::CancelOrderRequest& cancel_order_request) override {
      _gateways_by_name[cancel_order_request.request_info.destination]->send(cancel_order_request);
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
      // experimental
      const common::RequestInfo request_info = {
        .destination = "",
      };
      const common::Heartbeat heartbeat = {
        .opaque = 123,
      };
      const common::HeartbeatRequest request = {
        .request_info = request_info,
        .heartbeat = heartbeat,
      };
      for (auto& iter : _gateways)
        if (iter.ready())
          iter.send(request);
    }

   private:
    Dispatcher() = delete;
    Dispatcher(const Dispatcher&) = delete;
    Dispatcher& operator=(const Dispatcher&) = delete;

   private:
    T _strategy;
    libevent::Base _base;
    libevent::TimerEvent _timer;
    std::list<Gateway> _gateways;
    std::unordered_map<std::string, Gateway *> _gateways_by_name;
    std::unordered_set<Gateway *> _callbacks;
  };

 private:
  Controller() = delete;
  Controller(const Controller&) = delete;
  Controller& operator=(const Controller&) = delete;

 private:
  gateways_t _gateways;
};

}  // namespace client
}  // namespace quinclas
