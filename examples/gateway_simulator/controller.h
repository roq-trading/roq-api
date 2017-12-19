/* Copyright (c) 2017,  Hans Erik Thrane */

#pragma once

#include <glog/logging.h>

#include <quinclas/tradingapi.h>
#include <quinclas/io/libevent.h>

#include <list>
#include <string>
#include <unordered_map>

namespace examples {
namespace gateway_simulator {

class Client {
 public:
  class Writer {
    virtual void write(const void *buffer, size_t length) = 0;
  };
  typedef std::function<std::unique_ptr<Client>(Writer&)> Factory;
  virtual ~Client() {}
  virtual void on_read() = 0;
};


class Connection : public Client::Writer {
 public:
  typedef std::function<void(Connection *)> Finalizer;
  Connection(quinclas::io::libevent::BufferEvent&& buffer_event, Client::Factory factory, Finalizer finalizer)
      : _buffer_event(std::move(buffer_event)), _client(factory(*this)), _finalizer(finalizer) {
    _buffer_event.setcb(on_read, nullptr, on_error, this);
    _buffer_event.enable(EV_READ);
  }

 private:
  static void on_read(struct bufferevent *bev, void *arg) {
    // TODO(thraneh): forward to _client
  }
  static void on_error(struct bufferevent *bev, short what, void *arg) {  // NOLINT
    auto& self = *reinterpret_cast<Connection *>(arg);
    self._finalizer(&self);  // garbage collect
  }

 private:
  void write(const void *buffer, size_t length) override {
    _buffer_event.write(buffer, length);
  }

 private:
  quinclas::io::libevent::BufferEvent _buffer_event;
  std::unique_ptr<Client> _client;
  Finalizer _finalizer;
};

class Service final : public quinclas::io::libevent::Listener::Handler {
 public:
  Service(quinclas::io::libevent::Base& base, quinclas::io::net::Socket&& socket, Client::Factory factory)
      : _listener(*this, base, 0, -1, std::move(socket)), _factory(factory) {}
  void refresh() {
    _zombies.clear();
  }

 private:
  void on_accept(quinclas::io::libevent::BufferEvent&& buffer_event) override {
    LOG(INFO) << "got connection";
    auto finalizer = [&](Connection *connection){ remove(connection); };
    auto connection = std::unique_ptr<Connection>(new Connection(std::move(buffer_event), _factory, finalizer));
    _connection.emplace(connection.get(), std::move(connection));
  }

 private:
  void remove(Connection *connection) {
    LOG(INFO) << "removing connection";
    auto iter = _connection.find(connection);
    assert(iter != _connection.end());
    _zombies.push_back(std::move((*iter).second));
    _connection.erase(iter);
  }

 private:
  quinclas::io::libevent::Listener _listener;
  Client::Factory _factory;
  std::unordered_map<Connection *, std::unique_ptr<Connection> > _connection;
  std::list<std::unique_ptr<Connection> > _zombies;
};

class Controller final : public quinclas::io::libevent::TimerEvent::Handler {
 public:
  explicit Controller(const std::unordered_map<std::string, Client::Factory>& handlers)
      : _timer(*this, _base) {
    for (auto iter : handlers) {
      unlink(iter.first.c_str());
      quinclas::io::net::Address address(iter.first.c_str());
      quinclas::io::net::Socket socket(PF_LOCAL, SOCK_STREAM, 0);
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
  quinclas::io::libevent::Base _base;
  quinclas::io::libevent::TimerEvent _timer;
  std::list<std::unique_ptr<Service> > _services;
};

}  // namespace gateway_simulator
}  // namespace examples
