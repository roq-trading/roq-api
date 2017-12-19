/* Copyright (c) 2017,  Hans Erik Thrane */

#pragma once

#include <glog/logging.h>

#include <quinclas/tradingapi.h>
#include <quinclas/io/libevent.h>

#include <unordered_map>
#include <list>
#include <string>

namespace examples {
namespace gateway_simulator {

class Client {
 public:
  typedef std::function<void(Client *)> Remove;
  typedef std::function<std::unique_ptr<Client>(quinclas::io::libevent::BufferEvent&&, Remove remove)> Factory;
  virtual ~Client() {}
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
    auto client = _factory(std::move(buffer_event), [&](Client *client){ remove(client); });
    _clients.emplace(client.get(), std::move(client));
  }

 private:
  void remove(Client *client) {
    LOG(INFO) << "removing connection";
    auto iter = _clients.find(client);
    assert(iter != _clients.end());
    _zombies.push_back(std::move((*iter).second));
    _clients.erase(iter);
  }

 private:
  quinclas::io::libevent::Listener _listener;
  Client::Factory _factory;
  std::unordered_map<Client *, std::unique_ptr<Client> > _clients;
  std::list<std::unique_ptr<Client> > _zombies;
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
