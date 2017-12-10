/* Copyright (c) 2017, Hans Erik Thrane */

#pragma once

#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/dns.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/thread.h>

#include <glog/logging.h>

#include <quinclas/io/net.h>

// FIXME(thraneh): remove dependency on net::

namespace quinclas {
namespace io {
namespace libevent {

class Thread {
 public:
  static void use() {
#if EVTHREAD_USE_PTHREADS_IMPLEMENTED == 1
    if (evthread_use_pthreads() < 0) {
      LOG(WARNING) << "evthread_use_pthreads() failed";
      throw std::runtime_error("unable to use pthreads");
    }
#else
#error "threading not supported"
#endif
    }
};

class Base {
 public:
  Base() : _base(event_base_new()) {
    if (_base == nullptr) {
      LOG(WARNING) << "event_base_new() failed";
      throw std::bad_alloc();
    }
  }
  Base(Base&& rhs) : _base(rhs._base) {}
  ~Base() {
    if (_base != nullptr)
      event_base_free(_base);
  }
  operator struct event_base *() const { return _base; }
  struct event_base *get_raw() const { return _base; }
  void dispatch() {
    if (event_base_dispatch(_base) < 0) {
      LOG(WARNING) << "event_base_dispatch() failed";
      throw std::runtime_error("event_base_dispatch() failed");
    }
  }
  void loop(int flags) {
    if (event_base_loop(_base, flags) < 0) {
      LOG(WARNING) << "event_base_loop() failed";
      throw std::runtime_error("event_base_loop() failed");
    }
  }
  void loopexit(const struct timeval *timeout = nullptr) {
    if (event_base_loopexit(_base, timeout) < 0) {
      LOG(WARNING) << "event_base_loopexit() failed";
      throw std::runtime_error("event_base_loopexit() failed");
    }
  }
  void loopbreak() {
    if (event_base_loopbreak(_base) < 0) {
      LOG(WARNING) << "event_base_loopbreak() failed";
      throw std::runtime_error("event_base_loopbreak() failed");
    }
  }

 private:
  Base(Base const&) = delete;
  Base& operator=(Base const&) = delete;
  struct event_base *_base;
};

class DNSBase {
 public:
  DNSBase(Base& base, int initialize_nameservers) :
    _evdns_base(evdns_base_new(base.get_raw(), initialize_nameservers)) {
    if (_evdns_base == nullptr) {
      LOG(WARNING) << "evdns_base_new() failed";
      throw std::bad_alloc();
    }
  }
  ~DNSBase() {
    if (_evdns_base != nullptr) {
      int fail_requests = 1;
      evdns_base_free(_evdns_base, fail_requests);
    }
  }
struct evdns_base *get_raw() { return _evdns_base; }
 private:
  struct evdns_base *_evdns_base;
};

// TODO(thraneh): evbuffer_enable_locking
// TODO(thraneh): evbuffer_defer_callbacks ?

class Buffer {
 public:
  Buffer() : _evbuffer(evbuffer_new()) {
    if (_evbuffer == nullptr) {
      LOG(WARNING) << "evbuffer_new() failed";
      throw std::bad_alloc();
    }
  }
  Buffer(Buffer&& rhs) : _evbuffer(std::move(rhs._evbuffer)) {}
  ~Buffer() {
    if (_evbuffer != nullptr)
      evbuffer_free(_evbuffer);
  }
  struct evbuffer *get_raw() { return _evbuffer; }
  size_t length() {
    return evbuffer_get_length(_evbuffer);
  }
  unsigned char *pullup(ev_ssize_t size) {
    return evbuffer_pullup(_evbuffer, size);
  }
  void drain(size_t length) {
    if (evbuffer_drain(_evbuffer, length) < 0) {
      LOG(WARNING) << "evbuffer_drain() failed";
      throw std::runtime_error("failed to drain the event buffer");
    }
  }
  void add(const void *data, size_t datlen) {
    if (evbuffer_add(_evbuffer, data, datlen) < 0) {
      LOG(WARNING) << "evbuffer_add() failed";
      throw std::runtime_error("failed to add the event buffer");
    }
  }

 private:
  Buffer(Buffer const&) = delete;
  Buffer& operator=(Buffer const&) = delete;
  struct evbuffer *_evbuffer;
};

class Event {
 public:
  Event(Base& base, int fd, short events, event_callback_fn callback, void *cbarg) :  // NOLINT short
    _event(event_new(base.get_raw(), fd, events, callback, cbarg)) {
    if (_event == nullptr) {
      LOG(WARNING) << "event_new() failed";
      throw std::bad_alloc();
    }
  }
  ~Event() {
    if (_event != nullptr)
      event_free(_event);
  }
  void add(struct timeval *timeout = nullptr) {
    if (event_add(_event, timeout) < 0) {
      LOG(WARNING) << "event_add() failed";
      throw std::runtime_error("Unable to add event");
    }
  }

 private:
  struct event *_event;
};

class TimerEvent : public Event {
 public:
  class Handler {
   public:
    virtual void on_timer() = 0;
  };
  TimerEvent(Handler& handler, Base& base) :
    Event(base, -1, EV_PERSIST, callback, &handler) {}

 private:
  TimerEvent() = delete;
  TimerEvent(TimerEvent const&) = delete;
  TimerEvent& operator=(TimerEvent const&) = delete;
  static void callback(evutil_socket_t, short, void *cbarg) {  // NOLINT short
    reinterpret_cast<Handler*>(cbarg)->on_timer();
  }
};

class SignalEvent : public Event {
 public:
  class Handler {
   public:
    virtual void on_signal(int signal) = 0;
  };
  SignalEvent(Handler& handler, Base& base, int signal) :
    Event(base, signal, EV_SIGNAL|EV_PERSIST, callback, &handler) {}

 private:
  SignalEvent() = delete;
  SignalEvent(SignalEvent const&) = delete;
  SignalEvent& operator=(SignalEvent const&) = delete;
  static void callback(evutil_socket_t sig, short events, void *cbarg) {  // NOLINT short
    reinterpret_cast<Handler*>(cbarg)->on_signal(sig);
  }
};

// TODO(thraneh): BEV_OPT_THREADSAFE ?
//
class BufferEvent {
 public:
  // BufferEvent(Base& base, DNSBase& dns_base, int family, const char *hostname, int port);
  BufferEvent(Base& base, io::net::Socket&& socket, int options = 0) :
    _socket(std::move(socket)),
    _bufferevent(bufferevent_socket_new(base.get_raw(), _socket.get_raw(), options)) {
    if (_bufferevent == nullptr) {
      LOG(WARNING) << "bufferevent_socket_new() failed";
      throw std::bad_alloc();
    }
  }
  BufferEvent(BufferEvent&& rhs) :
    _socket(std::move(rhs._socket)),
    _bufferevent(rhs._bufferevent) {}
  virtual ~BufferEvent() {
    if (_bufferevent != nullptr)
      bufferevent_free(_bufferevent);
  }
  operator struct bufferevent *() { return _bufferevent; }
  struct bufferevent *get_raw() const { return _bufferevent; }
  void setcb(bufferevent_data_cb readcb, bufferevent_data_cb writecb, bufferevent_event_cb eventcb, void *cbarg) {
    bufferevent_setcb(_bufferevent, readcb, writecb, eventcb, cbarg);
  }
  void enable(short events) {  // NOLINT short
    bufferevent_enable(_bufferevent, events);
  }
  void read(Buffer& buffer) {
    if (bufferevent_read_buffer(_bufferevent, buffer.get_raw()) < 0) {
      LOG(WARNING) << "bufferevent_read_buffer() failed";
      throw std::runtime_error("unable to read data");
    }
  }
  size_t read(void* data, size_t length) {
    return bufferevent_read(_bufferevent, data, length);
  }
  void write(const void* data, size_t length) {
    const auto res = bufferevent_write(_bufferevent, data, length);
    if (res != 0) {
      LOG(ERROR) << "bufferevent_write() failed";
      throw std::runtime_error("unable to write");
    }
  }
  void write(Buffer& buffer) {
    if (bufferevent_write_buffer(_bufferevent, buffer.get_raw()) < 0) {
      LOG(ERROR) << "bufferevent_write_buffer() failed";
      throw std::runtime_error("unable to write");
    }
  }
  void connect(const struct sockaddr *address, int address_length) {
    if (bufferevent_socket_connect(_bufferevent, address, address_length) < 0) {
      LOG(ERROR) << "bufferevent_socket_connect() failed";
      throw std::runtime_error("unable to connect");
    }
  }
  void connect(DNSBase& dns_base, int family, const char *hostname, int port) {
    if (bufferevent_socket_connect_hostname(_bufferevent, dns_base.get_raw(), family, hostname, port) < 0) {
      LOG(ERROR) << "bufferevent_socket_connect_hostname() failed";
      throw std::runtime_error("unable to connect");
    }
  }
  template <typename T>
  void connect(const T& address) {
    if (bufferevent_socket_connect(_bufferevent, address.raw(), address.size()) < 0) {
      LOG(ERROR) << "bufferevent_socket_connect() failed";
      throw std::runtime_error("unable to connect");
    }
  }

 private:
  BufferEvent() = delete;
  BufferEvent(BufferEvent const&) = delete;
  BufferEvent& operator=(BufferEvent const&) = delete;
  io::net::Socket _socket;
  struct bufferevent *_bufferevent;
};

class Listener {
 public:
  class Handler {
   public:
    virtual void on_accept(BufferEvent&& buffer) = 0;
  };
  Listener(Handler& handler, Base& base, int flags, int backlog, io::net::Socket&& socket) :
    _handler(handler),
    _base(base),
    _socket(std::move(socket)),
    _evconnlistener(evconnlistener_new(base.get_raw(), callback, this, flags, backlog, _socket.get_raw())) {
    if (_evconnlistener == nullptr) {
      LOG(WARNING) << "evconnlistener_new() failed";
      throw std::bad_alloc();
    }
    DCHECK_EQ(_socket.non_blocking(), true) << "Socket must be non-blocking";
  }
  ~Listener() {
    if (_evconnlistener != nullptr)
      evconnlistener_free(_evconnlistener);
  }

 private:
  Listener() = delete;
  Listener(Listener const&) = delete;
  Listener& operator=(Listener const&) = delete;
  static void callback(struct evconnlistener *, evutil_socket_t fd, struct sockaddr *, int socklen, void *cbarg) {
    auto& listener = *reinterpret_cast<Listener*>(cbarg);
    io::net::Socket socket(fd);
    DCHECK_EQ(socket.non_blocking(), true) << "Socket must be non-blocking";
    socket.no_delay(true);  // FIXME(thraneh): maybe not here?
    // LOG(INFO) << "Accepted new connection from " << io::net::Utilities::inet_ntop(AF_INET, &(address.sin_addr));
    BufferEvent buffer(listener._base, std::move(socket));
    listener._handler.on_accept(std::move(buffer));
  }
  Handler& _handler;
  Base& _base;
  io::net::Socket _socket;
  struct evconnlistener *_evconnlistener;
};

}  // namespace libevent
}  // namespace io
}  // namespace quinclas
