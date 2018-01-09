/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/dns.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/thread.h>

#include <glog/logging.h>

#include <quinclas/net.h>

#include <cstring>

namespace quinclas {
namespace libevent {

// Thread

class Thread final {
 public:
  static void use_pthreads() {
#if EVTHREAD_USE_PTHREADS_IMPLEMENTED == 1
    if (evthread_use_pthreads() < 0) {
      LOG(WARNING) << "libevent: evthread_use_pthreads() failed";
      throw std::runtime_error("libevent: evthread_use_pthreads");
    }
#else
#error "libevent does not support pthread operations"
#endif
    }
};

// Base

class Base final {
 public:
  Base() : _base(event_base_new()) {
    if (_base == nullptr) {
      LOG(WARNING) << "libevent: event_base_new() failed";
      throw std::runtime_error("libevent: event_base_new");
    }
  }
  ~Base() {
    if (_base != nullptr)
      event_base_free(_base);
  }
  Base(Base&& rhs) : _base(rhs._base) {
    rhs._base = nullptr;
  }
  Base& operator=(Base&& rhs) {
    if (_base != nullptr)
      event_base_free(_base);
    _base = rhs._base;
    rhs._base = nullptr;
    return *this;
  }
  struct event_base *raw() const { return _base; }
  void dispatch() {
    if (event_base_dispatch(_base) < 0) {
      LOG(WARNING) << "libevent: event_base_dispatch() failed";
      throw std::runtime_error("libevent: event_base_dispatch");
    }
  }
  void loop(int flags) {
    if (event_base_loop(_base, flags) < 0) {
      LOG(WARNING) << "libevent: event_base_loop() failed";
      throw std::runtime_error("libevent: event_base_loop");
    }
  }
  void loopexit(const struct timeval *timeout = nullptr) {
    if (event_base_loopexit(_base, timeout) < 0) {
      LOG(WARNING) << "libevent: event_base_loopexit() failed";
      throw std::runtime_error("libevent: event_base_loopexit");
    }
  }
  void loopbreak() {
    if (event_base_loopbreak(_base) < 0) {
      LOG(WARNING) << "libevent: event_base_loopbreak() failed";
      throw std::runtime_error("libevent: event_base_loopbreak");
    }
  }

 private:
  Base(const Base&) = delete;
  Base& operator=(const Base&) = delete;
  struct event_base *_base;
};

// DNSBase

class DNSBase final {
 public:
  DNSBase(struct event_base *base, int initialize_nameservers)
      : _evdns_base(evdns_base_new(base, initialize_nameservers)) {
    if (_evdns_base == nullptr) {
      LOG(WARNING) << "libevent: evdns_base_new() failed";
      throw std::runtime_error("libevent: evdns_base_new");
    }
  }
  DNSBase(Base& base, int initialize_nameservers) : DNSBase(base.raw(), initialize_nameservers) {}
  ~DNSBase() {
    if (_evdns_base != nullptr) {
      int fail_requests = 1;
      evdns_base_free(_evdns_base, fail_requests);
    }
  }
  DNSBase(DNSBase&& rhs) : _evdns_base(rhs._evdns_base) {
    rhs._evdns_base = nullptr;
  }
  DNSBase& operator=(DNSBase&& rhs) {
    if (_evdns_base != nullptr) {
      int fail_requests = 1;
      evdns_base_free(_evdns_base, fail_requests);
    }
    _evdns_base = rhs._evdns_base;
    rhs._evdns_base = nullptr;
    return *this;
  }
  struct evdns_base *raw() const { return _evdns_base; }

 private:
  DNSBase() = delete;
  DNSBase(const DNSBase&) = delete;
  DNSBase& operator=(const DNSBase&) = delete;
  struct evdns_base *_evdns_base;
};

// Event

class Event {
 public:
  Event(struct event_base *base, int fd, short events, event_callback_fn callback, void *cbarg)  // NOLINT
      : _event(event_new(base, fd, events, callback, cbarg)) {
    if (_event == nullptr) {
      LOG(WARNING) << "libevent: event_new() failed";
      throw std::runtime_error("libevent: event_new");
    }
  }
  Event(Base& base, int fd, short events, event_callback_fn callback, void *cbarg)  // NOLINT
      : Event(base.raw(), fd, events, callback, cbarg) {}
  ~Event() {
    if (_event != nullptr)
      event_free(_event);
  }
  void add(const struct timeval *timeout = nullptr) {
    if (event_add(_event, timeout) < 0) {
      LOG(WARNING) << "libevent: event_add() failed";
      throw std::runtime_error("libevent: event_add");
    }
  }
  void add(const struct timeval& timeout) {
    add(&timeout);
  }

 private:
  Event() = delete;
  Event(const Event&) = delete;
  Event& operator=(const Event&) = delete;
  struct event *_event;
};

// TimerEvent

class TimerEvent final
    : public Event {
 public:
  class Handler {
   public:
    virtual void on_timer() = 0;
  };
  TimerEvent(Handler& handler, struct event_base *base) : Event(base, -1, EV_PERSIST, callback, &handler) {}
  TimerEvent(Handler& handler, Base& base) : Event(base, -1, EV_PERSIST, callback, &handler) {}

 private:
  TimerEvent() = delete;
  TimerEvent(TimerEvent const&) = delete;
  TimerEvent& operator=(TimerEvent const&) = delete;
  static void callback(evutil_socket_t, short, void *cbarg) {  // NOLINT
    try {
      reinterpret_cast<Handler*>(cbarg)->on_timer();
    } catch (std::exception& e) {
      LOG(FATAL) << "Unexpected: unhandled exception: " << e.what();
    } catch (...) {
      LOG(FATAL) << "Unexpected: unhandled exception";
    }
  }
};

// SignalEvent

class SignalEvent final
    : public Event {
 public:
  class Handler {
   public:
    virtual void on_signal(int signal) = 0;
  };
  SignalEvent(Handler& handler, struct event_base *base, int signal)
      : Event(base, signal, EV_SIGNAL | EV_PERSIST, callback, &handler) {}
  SignalEvent(Handler& handler, Base& base, int signal)
      : Event(base, signal, EV_SIGNAL | EV_PERSIST, callback, &handler) {}

 private:
  SignalEvent() = delete;
  SignalEvent(SignalEvent const&) = delete;
  SignalEvent& operator=(SignalEvent const&) = delete;
  static void callback(evutil_socket_t sig, short events, void *cbarg) {  // NOLINT
    try {
      reinterpret_cast<Handler*>(cbarg)->on_signal(sig);
    } catch (std::exception& e) {
      LOG(FATAL) << "Unexpected: unhandled exception: " << e.what();
    } catch (...) {
      LOG(FATAL) << "Unexpected: unhandled exception";
    }
  }
};

// Buffer

class Buffer final {
 public:
  Buffer() : _evbuffer(evbuffer_new()) {
    if (_evbuffer == nullptr) {
      LOG(WARNING) << "libevent: evbuffer_new() failed";
      throw std::runtime_error("libevent: evbuffer_new");
    }
  }
  ~Buffer() {
    if (_evbuffer != nullptr)
      evbuffer_free(_evbuffer);
  }
  Buffer(Buffer&& rhs) : _evbuffer(rhs._evbuffer) {
    rhs._evbuffer = nullptr;
  }
  Buffer& operator=(Buffer&& rhs) {
    if (_evbuffer != nullptr)
      evbuffer_free(_evbuffer);
    _evbuffer = rhs._evbuffer;
    rhs._evbuffer = nullptr;
    return *this;
  }
  struct evbuffer *raw() const { return _evbuffer; }
  size_t length() const {
    return evbuffer_get_length(_evbuffer);
  }
  void enable_locking(void *lock = nullptr) {
    if (evbuffer_enable_locking(_evbuffer, lock) < 0) {
      LOG(WARNING) << "libevent: evbuffer_enable_locking() failed";
      throw std::runtime_error("libevent: evbuffer_enable_locking");
    }
  }
  void lock() {
    evbuffer_lock(_evbuffer);
  }
  void unlock() {
    evbuffer_unlock(_evbuffer);
  }
  unsigned char *pullup(ev_ssize_t size) {
    return evbuffer_pullup(_evbuffer, size);
  }
  void drain(size_t length) {
    if (evbuffer_drain(_evbuffer, length) < 0) {
      LOG(WARNING) << "libevent: evbuffer_drain() failed";
      throw std::runtime_error("libevent: evbuffer_drain");
    }
  }
  void add(const void *data, size_t datlen) {
    if (evbuffer_add(_evbuffer, data, datlen) < 0) {
      LOG(WARNING) << "libevent: evbuffer_add() failed";
      throw std::runtime_error("libevent: evbuffer_add");
    }
  }

 private:
  Buffer(const Buffer&) = delete;
  Buffer& operator=(const Buffer&) = delete;
  struct evbuffer *_evbuffer;
};

// BufferEvent
// - BEV_OPT_THREADSAFE

class BufferEvent final {
 public:
  BufferEvent(struct event_base *base, int fd, int options = 0)
      : _bufferevent(bufferevent_socket_new(base, fd, options)) {
    if (_bufferevent == nullptr) {
      LOG(WARNING) << "libevent: bufferevent_socket_new() failed";
      throw std::runtime_error("libevent: bufferevent_socket_new");
    }
  }
  BufferEvent(Base& base, int fd, int options = 0) : BufferEvent(base.raw(), fd, options) {}
  BufferEvent(struct event_base *base, net::Socket&& socket, int options = 0)
      : BufferEvent(base, socket.release(), options | BEV_OPT_CLOSE_ON_FREE) {}
  BufferEvent(Base& base, net::Socket&& socket, int options = 0)
      : BufferEvent(base.raw(), socket.release(), options | BEV_OPT_CLOSE_ON_FREE) {}
  ~BufferEvent() {
    if (_bufferevent != nullptr)
      bufferevent_free(_bufferevent);
  }
  BufferEvent(BufferEvent&& rhs) : _bufferevent(rhs._bufferevent) {
    rhs._bufferevent = nullptr;
  }
  BufferEvent& operator=(BufferEvent&& rhs) {
    if (_bufferevent != nullptr)
      bufferevent_free(_bufferevent);
    _bufferevent = rhs._bufferevent;
    rhs._bufferevent = nullptr;
    return *this;
  }
  struct bufferevent *raw() const { return _bufferevent; }
  void setcb(bufferevent_data_cb readcb, bufferevent_data_cb writecb, bufferevent_event_cb eventcb, void *cbarg) {
    bufferevent_setcb(_bufferevent, readcb, writecb, eventcb, cbarg);
  }
  void enable(short events) {  // NOLINT
    bufferevent_enable(_bufferevent, events);
  }
  void read(struct evbuffer *buffer) {
    if (bufferevent_read_buffer(_bufferevent, buffer) < 0) {
      LOG(WARNING) << "libevent: bufferevent_read_buffer() failed";
      throw std::runtime_error("libevent: bufferevent_read_buffer");
    }
  }
  void read(Buffer& buffer) {
    read(buffer.raw());
  }
  size_t read(void* data, size_t length) {
    return bufferevent_read(_bufferevent, data, length);
  }
  void write(const void* data, size_t length) {
    const auto res = bufferevent_write(_bufferevent, data, length);
    if (res != 0) {
      LOG(ERROR) << "libevent: bufferevent_write() failed";
      throw std::runtime_error("libevent: bufferevent_write");
    }
  }
  void write(struct evbuffer *buffer) {
    if (bufferevent_write_buffer(_bufferevent, buffer) < 0) {
      LOG(ERROR) << "libevent: bufferevent_write_buffer() failed";
      throw std::runtime_error("libevent: bufferevent_write_buffer");
    }
  }
  void write(Buffer& buffer) {
    write(buffer.raw());
  }
  void connect(const struct sockaddr *address, int address_length) {
    if (bufferevent_socket_connect(_bufferevent, address, address_length) < 0) {
      LOG(ERROR) << "libevent: bufferevent_socket_connect() failed";
      throw std::runtime_error("libevent: bufferevent_socket_connect");
    }
  }
  void connect(const net::Address& address) {
    connect(address.raw(), address.size());
  }
  void connect(struct evdns_base *dns_base, int family, const char *hostname, int port) {
    if (bufferevent_socket_connect_hostname(_bufferevent, dns_base, family, hostname, port) < 0) {
      LOG(ERROR) << "libevent: bufferevent_socket_connect_hostname() failed";
      throw std::runtime_error("libevent: bufferevent_socket_connect_hostname");
    }
  }
  void connect(DNSBase& dns_base, int family, const char *hostname, int port) {
    connect(dns_base.raw(), family, hostname, port);
  }

 private:
  BufferEvent() = delete;
  BufferEvent(const BufferEvent&) = delete;
  BufferEvent& operator=(const BufferEvent&) = delete;
  struct bufferevent *_bufferevent;
};

// Listener
// - LEV_OPT_THREADSAFE

class Listener final {
 public:
  class Handler {
   public:
    virtual void on_accept(BufferEvent&& buffer) = 0;
  };
  Listener(Handler& handler, struct event_base *base, int flags, int backlog, net::Socket&& socket,
           int buffer_event_create_flags)
      : _handler(handler),
        _base(base),
        _socket(std::move(socket)),
        _evconnlistener(evconnlistener_new(base, callback, this, flags, backlog, _socket.raw())),
        _buffer_event_create_flags(buffer_event_create_flags) {
    if (_evconnlistener == nullptr) {
      LOG(WARNING) << "libevent: evconnlistener_new() failed";
      throw std::runtime_error("libevent: evconnlistener_new");
    }
    DCHECK_EQ(_socket.non_blocking(), true) << "socket must be non-blocking";
  }
  Listener(Handler& handler, Base& base, int flags, int backlog, net::Socket&& socket,
           int buffer_event_create_flags)
      : Listener(handler, base.raw(), flags, backlog, std::move(socket), buffer_event_create_flags) {}
  ~Listener() {
    if (_evconnlistener != nullptr)
      evconnlistener_free(_evconnlistener);
  }

 private:
  Listener() = delete;
  Listener(const Listener&) = delete;
  Listener& operator=(const Listener&) = delete;
  static void callback(struct evconnlistener *, evutil_socket_t fd, struct sockaddr *, int socklen, void *cbarg) {
    try {
      auto& self = *reinterpret_cast<Listener*>(cbarg);
      BufferEvent bufferevent(self._base, fd, self._buffer_event_create_flags | BEV_OPT_CLOSE_ON_FREE);
      self._handler.on_accept(std::move(bufferevent));
    } catch (std::exception& e) {
      LOG(FATAL) << "Unexpected: unhandled exception: " << e.what();
    } catch (...) {
      LOG(FATAL) << "Unexpected: unhandled exception";
    }
  }
  Handler& _handler;
  struct event_base *_base;
  net::Socket _socket;  // TODO(thraneh): pass to evconnlistener and use LEV_OPT_CLOSE_ON_FREE
  struct evconnlistener *_evconnlistener;
  int _buffer_event_create_flags;
};

}  // namespace libevent
}  // namespace quinclas
