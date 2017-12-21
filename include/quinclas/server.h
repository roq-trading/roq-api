/* Copyright (c) 2017, Hans Erik Thrane */

#pragma once

#include <glog/logging.h>

#include <quinclas/tradingapi.h>
#include <quinclas/libevent.h>

#include <list>
#include <string>
#include <unordered_map>

namespace quinclas {
namespace server {

// Client
class Client {
 public:
  // Writer
  class Writer {
    virtual void write(const void *buffer, size_t length) = 0;
  };
  typedef std::function<std::unique_ptr<Client>(Writer&)> Factory;
  virtual ~Client() {}
  virtual void on_read() = 0;
};

// Connection
class Connection final : public Client::Writer {
 public:
  typedef std::function<void(Connection *)> Finalizer;
  Connection(libevent::BufferEvent&& buffer_event, Client::Factory factory, Finalizer finalizer)
      : _buffer_event(std::move(buffer_event)), _client(factory(*this)), _finalizer(finalizer) {
    _buffer_event.setcb(on_read, nullptr, on_error, this);
    _buffer_event.enable(EV_READ);
  }

 private:
  void on_error(int reason) {
    assert(0 == (reason & BEV_EVENT_CONNECTED));  // the listener should only pass already connected sockets
    _finalizer(this);  // garbage collect
  }
  void on_read() {
    // TODO(thraneh): parse and dispatch
  }
  void write(const void *buffer, size_t length) override {
    try {
      _buffer_event.write(buffer, length);
    } catch (std::runtime_error& e) {  // TODO(thraneh): maybe a more specific exception type?
      LOG(WARNING) << "connection: caught exception, what=\"" << e.what() << "\"";
      // TODO(thraneh): how to remove connection?
    }
  }

 private:
  static void on_error(struct bufferevent *bev, short what, void *arg) {  // NOLINT
    reinterpret_cast<Connection *>(arg)->on_error(what);
  }
  static void on_read(struct bufferevent *bev, void *arg) {
    reinterpret_cast<Connection *>(arg)->on_read();
  }

 private:
  Connection() = delete;
  Connection(const Connection&) = delete;
  Connection& operator=(const Connection&) = delete;

 private:
  libevent::BufferEvent _buffer_event;
  std::unique_ptr<Client> _client;
  Finalizer _finalizer;
};

// Service
class Service final : public libevent::Listener::Handler {
 public:
  Service(libevent::Base& base, net::Socket&& socket, Client::Factory factory)
      : _listener(*this, base, 0, -1, std::move(socket)), _factory(factory) {}
  void refresh() {
    _zombies.clear();
  }

 private:
  void on_accept(libevent::BufferEvent&& buffer_event) override {
    LOG(INFO) << "service: got connection";
    auto finalizer = [this](Connection *connection){ remove(connection); };
    auto connection = std::unique_ptr<Connection>(new Connection(std::move(buffer_event), _factory, finalizer));
    _connection.emplace(connection.get(), std::move(connection));
  }

 private:
  void remove(Connection *connection) {
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
  Client::Factory _factory;
  std::unordered_map<Connection *, std::unique_ptr<Connection> > _connection;
  std::list<std::unique_ptr<Connection> > _zombies;
};

// Controller
class Controller final : public libevent::TimerEvent::Handler {
 public:
  explicit Controller(const std::unordered_map<std::string, Client::Factory>& handlers)
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
