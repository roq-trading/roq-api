/* Copyright (c) 2017, Hans Erik Thrane */

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <quinclas/client/event_dispatcher.h>
#include <quinclas/client/request_dispatcher.h>

#include <list>
#include <unordered_map>

#include "your/strategy.h"

DEFINE_string(local_address, "", "host-internal socket address (path)");

namespace {
template <typename T>
class Connection {
 public:
  class Handler {
   public:
    virtual void on_remove(const quinclas::io::libevent::BufferEvent& buffer) = 0;
  };
  Connection(Handler& handler, quinclas::io::libevent::BufferEvent&& bufferevent, T&& client) :
    _handler(handler),
    _bufferevent(std::move(bufferevent)),
    _client(std::move(client)) {
    _bufferevent.setcb(on_read, nullptr, on_error, this);
    _bufferevent.enable(EV_READ);
  }
  Connection(Connection&& rhs) :
    _handler(rhs._handler),
    _bufferevent(std::move(rhs._bufferevent)),
    _buffer(std::move(rhs._buffer)),
    _client(std::move(rhs._client)) {
  }

 private:
  static void on_read(struct bufferevent *bev, void *arg) {
    auto self = reinterpret_cast<Connection*>(arg);
    self->_bufferevent.read(self->_buffer);
    self->_client.on_read(self->_buffer);
  }
  static void on_error(struct bufferevent *bev, short what, void *arg) {  // NOLINT
    auto self = reinterpret_cast<Connection*>(arg);
    if (what & BEV_EVENT_EOF)
      LOG(INFO) << "client disconnected";
    else
      LOG(WARNING) << "socket error";
    self->_handler.on_remove(self->_bufferevent);
  }

 private:
  Connection() = delete;
  Connection(const Connection&) = delete;
  Connection& operator=(const Connection&) = delete;
  Handler& _handler;
  quinclas::io::libevent::BufferEvent _bufferevent;
  quinclas::io::libevent::Buffer _buffer;
  T _client;
};
}  // namespace

namespace {
template <typename T>
class Factory {};
}  // namespace

namespace {
template <typename T>
class Service :
  public quinclas::io::libevent::TimerEvent::Handler,
  public quinclas::io::libevent::Listener::Handler,
  public Connection<T>::Handler {
 public:
  Service(quinclas::io::libevent::Base& base, quinclas::io::net::Socket&& socket, Factory<T>&& factory) :
    _timer(*this, base),
    _listener(*this, base, 0, -1, std::move(socket)),
    _factory(std::move(factory)) {
    struct timeval timeout = { .tv_sec = 3 };
    _timer.add(&timeout);
  }

 protected:
  void on_accept(quinclas::io::libevent::BufferEvent&& bufferevent) override {
    LOG(INFO) << "Accept";
    _connections.emplace(bufferevent.raw(),
                         Connection<T>(*this, std::move(bufferevent), std::move(_factory.create())));
    LOG(INFO) << "Added client";
  }
  void on_remove(const quinclas::io::libevent::BufferEvent& buffer) override {
    LOG(INFO) << "Remove";
    auto iter = _connections.find(buffer.raw());
    if (iter == _connections.end()) {
      LOG(ERROR) << "Can't remove non-existing client";
    } else {
      LOG(INFO) << "Removing client";
      _zombied.push_back(std::move((*iter).second));
      _connections.erase(iter);
    }
  }
  void on_timer() override {
    LOG(INFO) << "Timer len(clients)=" << _connections.size() << ", len(zombied)=" << _zombied.size();
    if (!_zombied.empty()) {
      LOG(INFO) << "Removing " << _zombied.size() << " zombied client(s)";
      _zombied.clear();
    }
  }
 private:
  Service() = delete;
  Service(const Service&) = delete;
  Service& operator=(const Service&) = delete;
  quinclas::io::libevent::TimerEvent _timer;
  quinclas::io::libevent::Listener _listener;
  Factory<T> _factory;
  std::unordered_map<struct bufferevent *, Connection<T> > _connections;
  std::list<Connection<T> > _zombied;
};
}  // namespace

namespace {
class Client {
 public:
  explicit Client(int a) : _a(a) {}
  void on_read(quinclas::io::libevent::Buffer& buffer) {
    unsigned char *envelope = buffer.pullup(quinclas::common::Envelope::LENGTH);
    if (envelope) {
      // - magic
      // - type
      // - checksum
      // - length payload
      size_t length_payload = 1;
      envelope = buffer.pullup(quinclas::common::Envelope::LENGTH + length_payload);
      if (envelope) {
        unsigned char *payload = envelope + quinclas::common::Envelope::LENGTH;
        try {
          // parse flatbuffer and dispatch
        }
        catch (...) {
        }
        buffer.drain(quinclas::common::Envelope::LENGTH + length_payload);
      }
    }
  }

 private:
  Client() = delete;
  // Client(const Client&) = delete;
  // Client& operator=(const Client&) = delete;
  int _a;
};
}  // namespace

namespace {
template<>
class Factory<Client> {
 public:
  explicit Factory(int a) : _a(a) {
  }
  // Factory(Factory&& rhs) : _a(std::move(rhs._a)) {}
  Client create() {
    return Client(_a);
  }
 private:
  Factory() = delete;
  // Factory(const Factory&) = delete;
  // Factory& operator=(const Factory&) = delete;
  int _a;
};
}  // namespace

int main(int argc, char *argv[]) {
  // initialize logging library
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  // parse command-line options
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ShutDownCommandLineFlags();
  // validate command-line options
  if (FLAGS_local_address.empty())
    throw std::invalid_argument("local_address is missing");
  // we're ready
  LOG(INFO) << "===== START =====";
  // remove left-over unix domain socket (protect with pidfile?)
  unlink(FLAGS_local_address.c_str());
  // create host-internal socket address
  quinclas::io::net::Address address(FLAGS_local_address.c_str());
  // create and bind host-internal listen socket
  quinclas::io::net::Socket socket(PF_LOCAL, SOCK_STREAM, 0);
  socket.non_blocking(true);
  socket.bind(address);
  // create event base
  quinclas::io::libevent::Base base;
  // create service implementation
  Factory<Client> factory(123);
  Service<Client> service(base, std::move(socket), std::move(factory));
  // run event loop
  base.loop(EVLOOP_NO_EXIT_ON_EMPTY);
  // we're done
  LOG(INFO) << "===== STOP =====";
  return EXIT_SUCCESS;  // TODO(thraneh): event-loop should be able to stop with an error code
}
