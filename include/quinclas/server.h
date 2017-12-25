/* Copyright (c) 2017, Hans Erik Thrane */

// FIXME:
// - implement codec -- perhaps revisit Gateway interface
// -- why? not implemented

#pragma once

#include <glog/logging.h>

#include <quinclas/tradingapi.h>
#include <quinclas/libevent.h>
#include <quinclas/codec.h>

#include <list>
#include <string>
#include <unordered_map>

namespace quinclas {
namespace server {

// Connection
class Connection : public common::Strategy::Dispatcher {
 public:
  // Dispatcher
  class Dispatcher {
    virtual void send(const void *payload, const size_t length_payload) = 0;
  };
  typedef std::function<std::unique_ptr<Connection>(Dispatcher&)> Factory;
  virtual ~Connection() = default;
};

// Client
class Client final : public Connection::Dispatcher {
 public:
  typedef std::function<void(Client *)> Finalizer;
  Client(libevent::BufferEvent&& buffer_event, Connection::Factory factory, Finalizer finalizer)
      : _buffer_event(std::move(buffer_event)), _connection(factory(*this)), _finalizer(finalizer),
        _request_dispatcher(*_connection) {
    _buffer_event.setcb(on_read, nullptr, on_error, this);
    _buffer_event.enable(EV_READ);
  }

 private:
  void send(const void *payload, const size_t length_payload) override {
    common::Envelope::encode(_envelope, length_payload);
    _buffer.add(_envelope, sizeof(_envelope));
    _buffer.add(payload, length_payload);
    try {
      _buffer_event.write(_buffer);
    } catch (std::runtime_error& e) {  // TODO(thraneh): maybe a more specific exception type?
      LOG(WARNING) << "gateway: caught exception, what=\"" << e.what() << "\"";
      // FIXM(thraneh): remove connection...
      LOG(WARNING) << "failed write attempt -- unable to send the event";
      throw std::runtime_error("unable to send the request");
    }
  }

 private:
  void on_error(int reason) {
    assert(0 == (reason & BEV_EVENT_CONNECTED));  // the listener should only pass already connected sockets
    _finalizer(this);  // garbage collect
  }
  void on_read() {
      _buffer_event.read(_buffer);
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
        _request_dispatcher.dispatch_request(payload, length_payload);
        _buffer.drain(bytes);
      }
  }

 private:
  static void on_error(struct bufferevent *bev, short what, void *arg) {  // NOLINT
    reinterpret_cast<Client *>(arg)->on_error(what);
  }
  static void on_read(struct bufferevent *bev, void *arg) {
    reinterpret_cast<Client *>(arg)->on_read();
  }

 private:
  Client() = delete;
  Client(const Client&) = delete;
  Client& operator=(const Client&) = delete;

 private:
  libevent::BufferEvent _buffer_event;
  std::unique_ptr<Connection> _connection;
  Finalizer _finalizer;
  common::RequestDispatcher _request_dispatcher;
  libevent::Buffer _buffer;
  flatbuffers::FlatBufferBuilder _flat_buffer_builder;
  uint8_t _envelope[common::Envelope::LENGTH];
};

// Service
class Service final : public libevent::Listener::Handler {
 public:
  Service(libevent::Base& base, net::Socket&& socket, Connection::Factory factory)
      : _listener(*this, base, 0, -1, std::move(socket)), _factory(factory) {}
  void refresh() {
    _zombies.clear();
  }

 private:
  void on_accept(libevent::BufferEvent&& buffer_event) override {
    LOG(INFO) << "service: got connection";
    auto finalizer = [this](Client *connection){ remove(connection); };
    auto connection = std::unique_ptr<Client>(new Client(std::move(buffer_event), _factory, finalizer));
    _connection.emplace(connection.get(), std::move(connection));
  }

 private:
  void remove(Client *connection) {
    LOG(INFO) << "service: removing connection";
    auto iter = _connection.find(connection);
    assert(iter != _connection.end());
    _zombies.push_back(std::move((*iter).second));
    _connection.erase(iter);
  }

 private:
  Service() = delete;
  Service(const Service&) = delete;
  Service& operator=(const Service&) = delete;

 private:
  libevent::Listener _listener;
  Connection::Factory _factory;
  std::unordered_map<Client *, std::unique_ptr<Client> > _connection;
  std::list<std::unique_ptr<Client> > _zombies;
};

// Controller
class Controller final : public libevent::TimerEvent::Handler {
 public:
  explicit Controller(const std::unordered_map<std::string, Connection::Factory>& handlers)
      : _timer(*this, _base) {
    for (auto iter : handlers) {
      unlink(iter.first.c_str());
      net::Address address(iter.first.c_str());
      net::Socket socket(PF_LOCAL, SOCK_STREAM, 0);
      socket.non_blocking(true);
      socket.bind(address);
      _services.emplace_back(std::unique_ptr<Service>(new Service(_base, std::move(socket), iter.second)));
    }
  }
  void dispatch() {
    _timer.add({.tv_sec = 1});
    _base.loop(EVLOOP_NO_EXIT_ON_EMPTY);
  }

  /*
  void on(const common::TerminationEvent& event) override {
    // send_helper(event);
  }
  void on(const common::GatewayStatusEvent& event) override {
    send_helper(event);
  }
  void on(const common::ReferenceDataEvent& event) override {
    send_helper(event);
  }
  void on(const common::MarketStatusEvent& event) override {
    send_helper(event);
  }
  void on(const common::MarketByPriceEvent& event) override {
    send_helper(event);
  }
  void on(const common::SessionStatisticsEvent& event) override {
    send_helper(event);
  }
  void on(const common::DailyStatisticsEvent& event) override {
    send_helper(event);
  }
  void on(const common::CreateOrderAckEvent& event) override {
    send_helper(event);
  }
  void on(const common::ModifyOrderAckEvent& event) override {
    send_helper(event);
  }
  void on(const common::CancelOrderAckEvent& event) override {
    send_helper(event);
  }
  void on(const common::OrderUpdateEvent& event) override {
    send_helper(event);
  }
  void on(const common::TradeUpdateEvent& event) override {
    send_helper(event);
  }
  */

 private:
  void on_timer() override {
    for (auto& iter : _services)
      (*iter).refresh();
  }

 private:
  Controller() = delete;
  Controller(const Controller&) = delete;
  Controller& operator=(const Controller&) = delete;

 private:
  libevent::Base _base;
  libevent::TimerEvent _timer;
  std::list<std::unique_ptr<Service> > _services;
};

}  // namespace server
}  // namespace quinclas
