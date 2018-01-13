/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/dns.h>
#include <event2/event.h>
#include <event2/http.h>
#include <event2/listener.h>
#include <event2/thread.h>

#include <glog/logging.h>

#include <quinclas/net.h>

#include <chrono>
#include <cstring>
#include <list>
#include <string>
#include <utility>

namespace quinclas {
namespace libevent {

// generic utilities

static void unhandled_exception() {
  try {
    throw;
  } catch (std::exception& e) {
    LOG(FATAL) << "Unhandled exception, type=" << typeid(e).name() << ", what=" << e.what();
  } catch (...) {
    LOG(FATAL) << "Unhandled exception";
  }
}

class RuntimeError : public std::exception {
 public:
  explicit RuntimeError(std::string&& what) : _what(std::move(what)) {
    VLOG(1) << "RuntimeError, what=" << _what.c_str();
  }
  const char *what() const noexcept override { return _what.c_str(); }
 private:
  std::string _what;
};

// threading support

static void use_pthreads() {
#if EVTHREAD_USE_PTHREADS_IMPLEMENTED == 1
  if (evthread_use_pthreads() < 0)
    throw RuntimeError("evthread_use_pthreads");
#else
#error "libevent does not support pthread operations"
#endif
}

// Base

class Base final {
 public:
  Base() : _base(event_base_new()) {
    if (_base == nullptr)
      throw RuntimeError("event_base_new");
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
  struct event_base *get() const { return _base; }
  void dispatch() {
    if (event_base_dispatch(_base) < 0)
      throw RuntimeError("event_base_dispatch");
  }
  void loop(int flags) {
    if (event_base_loop(_base, flags) < 0)
      throw RuntimeError("event_base_loop");
  }
  void loopexit(const struct timeval *timeout = nullptr) {
    if (event_base_loopexit(_base, timeout) < 0)
      throw RuntimeError("event_base_loopexit");
  }
  void loopbreak() {
    if (event_base_loopbreak(_base) < 0)
      throw RuntimeError("event_base_loopbreak");
  }

 private:
  Base(const Base&) = delete;
  Base& operator=(const Base&) = delete;

 private:
  struct event_base *_base;
};

// DNSBase

class DNSBase final {
 public:
  DNSBase(struct event_base *base, int initialize_nameservers)
      : _evdns_base(evdns_base_new(base, initialize_nameservers)) {
    if (_evdns_base == nullptr)
      throw RuntimeError("evdns_base_new");
  }
  DNSBase(Base& base, int initialize_nameservers) : DNSBase(base.get(), initialize_nameservers) {}
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
  struct evdns_base *get() const { return _evdns_base; }

 private:
  DNSBase() = delete;
  DNSBase(const DNSBase&) = delete;
  DNSBase& operator=(const DNSBase&) = delete;

 private:
  struct evdns_base *_evdns_base;
};

// Timer

class Timer final {
 public:
  typedef std::function<void()> handler_t;
  Timer(struct event_base *base, handler_t&& handler)
      : _handler(std::move(handler)),
        _event(event_new(base, -1, EV_PERSIST, callback, &_handler)) {
    if (_event == nullptr)
      throw RuntimeError("event_new");
  }
  Timer(Base& base, handler_t&& handler) : Timer(base.get(), std::move(handler)) {}
  ~Timer() {
    if (_event != nullptr)
      event_free(_event);
  }
  template <typename T>
  void add(const T duration) {
    const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    struct timeval timeval = {
      .tv_sec = static_cast<time_t>(milliseconds.count() / 1000),
      .tv_usec = static_cast<suseconds_t>((milliseconds.count() % 1000) * 1000),
    };
    if (event_add(_event, &timeval) < 0)
      throw RuntimeError("event_add");
  }

 private:
  static void callback(evutil_socket_t, short, void *cbarg) {  // NOLINT
    try {
      (*reinterpret_cast<handler_t *>(cbarg))();
    } catch (...) {
      unhandled_exception();
    }
  }

 private:
  Timer() = delete;
  Timer(Timer const&) = delete;
  Timer& operator=(Timer const&) = delete;

 private:
  handler_t _handler;
  struct event *_event;
};

// SignalEvent

class SignalEvent final {
 public:
  typedef std::function<void(int)> handler_t;
  SignalEvent(struct event_base *base, int signal, handler_t&& handler)
      : _handler(std::move(handler)),
        _event(event_new(base, signal, EV_SIGNAL | EV_PERSIST, callback, &_handler)) {
    if (_event == nullptr)
      throw RuntimeError("event_new");
  }
  SignalEvent(Base& base, int signal, handler_t&& handler)
      : SignalEvent(base.get(), signal, std::move(handler)) {}

 private:
  static void callback(evutil_socket_t sig, short events, void *cbarg) {  // NOLINT
    try {
      (*reinterpret_cast<handler_t *>(cbarg))(sig);
    } catch (...) {
      unhandled_exception();
    }
  }

 private:
  SignalEvent() = delete;
  SignalEvent(SignalEvent const&) = delete;
  SignalEvent& operator=(SignalEvent const&) = delete;

 private:
  handler_t _handler;
  struct event *_event;
};

// Buffer

class Buffer final {
 public:
  Buffer() : _evbuffer(evbuffer_new()) {
    if (_evbuffer == nullptr)
      throw RuntimeError("evbuffer_new");
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
  struct evbuffer *get() const { return _evbuffer; }
  size_t length() const {
    return evbuffer_get_length(_evbuffer);
  }
  void enable_locking(void *lock = nullptr) {
    if (evbuffer_enable_locking(_evbuffer, lock) < 0)
      throw RuntimeError("evbuffer_enable_locking");
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
    if (evbuffer_drain(_evbuffer, length) < 0)
      throw RuntimeError("evbuffer_drain");
  }
  void add(const void *data, size_t datlen) {
    if (evbuffer_add(_evbuffer, data, datlen) < 0)
      throw RuntimeError("evbuffer_add");
  }

 private:
  Buffer(const Buffer&) = delete;
  Buffer& operator=(const Buffer&) = delete;

 private:
  struct evbuffer *_evbuffer;
};

// BufferEvent
// - BEV_OPT_THREADSAFE

class BufferEvent final {
 public:
  typedef std::function<void()> read_handler_t;
  typedef std::function<void()> write_handler_t;
  typedef std::function<void(int)> error_handler_t;
  BufferEvent(struct event_base *base, int fd, int options)
      : _bufferevent(bufferevent_socket_new(base, fd, options)) {
    if (_bufferevent == nullptr)
      throw RuntimeError("bufferevent_socket_new");
  }
  BufferEvent(Base& base, int fd, int options)
      : BufferEvent(base.get(), fd, options) {}
  BufferEvent(struct event_base *base, net::Socket&& socket, int options)
      : BufferEvent(base, socket.release(), options | BEV_OPT_CLOSE_ON_FREE) {}
  BufferEvent(Base& base, net::Socket&& socket, int options)
      : BufferEvent(base.get(), socket.release(), options | BEV_OPT_CLOSE_ON_FREE) {}
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
  struct bufferevent *get() const { return _bufferevent; }
  void setcb(read_handler_t&& read_handler, write_handler_t&& write_handler,
             error_handler_t&& error_handler) {
    _read_handler = std::move(read_handler);
    _write_handler = std::move(write_handler);
    _error_handler = std::move(error_handler);
    bufferevent_setcb(_bufferevent, read_callback, write_callback, error_callback, this);
  }
  void setcb(read_handler_t&& read_handler, error_handler_t&& error_handler) {
    setcb(std::move(read_handler), [](){}, std::move(error_handler));
  }
  void enable(short events) {  // NOLINT
    bufferevent_enable(_bufferevent, events);
  }
  void read(struct evbuffer *buffer) {
    if (bufferevent_read_buffer(_bufferevent, buffer) < 0)
      throw RuntimeError("bufferevent_read_buffer");
  }
  void read(Buffer& buffer) {
    read(buffer.get());
  }
  size_t read(void* data, size_t length) {
    return bufferevent_read(_bufferevent, data, length);
  }
  void write(const void* data, size_t length) {
    const auto res = bufferevent_write(_bufferevent, data, length);
    if (res != 0)
      throw RuntimeError("bufferevent_write");
  }
  void write(struct evbuffer *buffer) {
    if (bufferevent_write_buffer(_bufferevent, buffer) < 0)
      throw RuntimeError("bufferevent_write_buffer");
  }
  void write(Buffer& buffer) {
    write(buffer.get());
  }
  void connect(const struct sockaddr *address, int address_length) {
    if (bufferevent_socket_connect(_bufferevent, address, address_length) < 0)
      throw RuntimeError("bufferevent_socket_connect");
  }
  void connect(const net::Address& address) {
    connect(address.get(), address.size());
  }
  void connect(struct evdns_base *dns_base, int family, const char *hostname, int port) {
    if (bufferevent_socket_connect_hostname(_bufferevent, dns_base, family, hostname, port) < 0)
      throw RuntimeError("bufferevent_socket_connect_hostname");
  }
  void connect(DNSBase& dns_base, int family, const char *hostname, int port) {
    connect(dns_base.get(), family, hostname, port);
  }

 private:
  static void read_callback(struct bufferevent *bev, void *ctx) {
    try {
      const auto& self = *reinterpret_cast<BufferEvent*>(ctx);
      self._read_handler();
    } catch (...) {
      unhandled_exception();
    }
  }
  static void write_callback(struct bufferevent *bev, void *ctx) {
    try {
      const auto& self = *reinterpret_cast<BufferEvent*>(ctx);
      self._write_handler();
    } catch (...) {
      unhandled_exception();
    }
  }
  static void error_callback(struct bufferevent *bev, short what, void *ctx) {  // NOLINT
    try {
      const auto& self = *reinterpret_cast<BufferEvent*>(ctx);
      self._error_handler(what);
    } catch (...) {
      unhandled_exception();
    }
  }

 private:
  BufferEvent() = delete;
  BufferEvent(const BufferEvent&) = delete;
  BufferEvent& operator=(const BufferEvent&) = delete;

 private:
  struct bufferevent *_bufferevent;
  read_handler_t _read_handler;
  write_handler_t _write_handler;
  error_handler_t _error_handler;
};

// Listener
// - LEV_OPT_THREADSAFE

class Listener final {
 public:
  typedef std::function<void(BufferEvent&&)> handler_t;
  Listener(struct event_base *base, int flags, int backlog, net::Socket&& socket,
           int create_flags, handler_t&& handler)
      : _base(base),
        _socket(std::move(socket)),
        _evconnlistener(evconnlistener_new(base, callback, this, flags, backlog, _socket.get())),
        _create_flags(create_flags),
        _handler(std::move(handler)) {
    if (_evconnlistener == nullptr)
      throw RuntimeError("evconnlistener_new");
    DCHECK_EQ(_socket.non_blocking(), true) << "socket must be non-blocking";
  }
  Listener(Base& base, int flags, int backlog, net::Socket&& socket,
           int create_flags, handler_t&& handler)
      : Listener(base.get(), flags, backlog, std::move(socket),
                 create_flags, std::move(handler)) {}
  ~Listener() {
    if (_evconnlistener != nullptr)
      evconnlistener_free(_evconnlistener);
  }

 private:
  static void callback(struct evconnlistener *, evutil_socket_t fd, struct sockaddr *,
                       int socklen, void *cbarg) {
    try {
      const auto& self = *reinterpret_cast<Listener*>(cbarg);
      BufferEvent bufferevent(self._base, fd, self._create_flags | BEV_OPT_CLOSE_ON_FREE);
      self._handler(std::move(bufferevent));
    } catch (...) {
      unhandled_exception();
    }
  }

 private:
  Listener() = delete;
  Listener(const Listener&) = delete;
  Listener& operator=(const Listener&) = delete;

 private:
  struct event_base *_base;
  net::Socket _socket;  // TODO(thraneh): pass to evconnlistener and use LEV_OPT_CLOSE_ON_FREE
  struct evconnlistener *_evconnlistener;
  int _create_flags;
  handler_t _handler;
};

// HTTPRequest

class HTTPRequest final {
 public:
  explicit HTTPRequest(struct evhttp_request *evhttp_request)
      : _evhttp_request(evhttp_request) {
    evhttp_request_own(_evhttp_request);  // FIXME(thraneh): this doesn't work?
  }
  ~HTTPRequest() {
    /* FIXME(thraneh): see comment about evhttp_request_own
    if (evhttp_request_is_owned(_evhttp_request) == 1)
      if (_evhttp_request != nullptr)
        evhttp_request_free(_evhttp_request);
    */
  }
  enum evhttp_cmd_type get_command() {
    return evhttp_request_get_command(_evhttp_request);
  }
  std::string get_uri() {
    return evhttp_request_get_uri(_evhttp_request);
  }
  void send_error(int error, const char *reason) {
    evhttp_send_error(_evhttp_request, error, reason);
  }
  void send_reply(int code, const char *reason, struct evbuffer *databuf) {
    evhttp_send_reply(_evhttp_request, code, reason, databuf);
  }
  void send_reply(int code, const char *reason, Buffer& buffer) {
    send_reply(code, reason, buffer.get());
  }
  void add_output_header(const char *key, const char *value) {
    if (evhttp_add_header(evhttp_request_get_output_headers(_evhttp_request), key, value) < 0)
      throw RuntimeError("evhttp_add_header");
  }
  /*
  struct evbuffer *get_output_buffer() {
    return evhttp_request_get_output_buffer(_evhttp_request);
  }
  */
  // const struct evhttp_uri *evhttp_request_get_evhttp_uri(const struct evhttp_request *req);
  // enum evhttp_cmd_type evhttp_request_get_command(const struct evhttp_request *req);
  // struct evkeyvalq *evhttp_request_get_input_headers(struct evhttp_request *req);
  // struct evhttp_uri *evhttp_uri_parse(const char *source_uri);

 private:
  HTTPRequest() = delete;
  HTTPRequest(const HTTPRequest&) = delete;
  HTTPRequest& operator=(const HTTPRequest&) = delete;

 private:
  struct evhttp_request *_evhttp_request;
  bool _own;  // TODO(thraneh): any scenarios where we really this?
};

// HTTP

class HTTP final {
 public:
  explicit HTTP(struct event_base *base)
      : _evhttp(evhttp_new(base)) {
    if (_evhttp == nullptr)
      throw RuntimeError("evhttp_new");
  }
  explicit HTTP(libevent::Base& base)
      : HTTP(base.get()) {}
  ~HTTP() {
    if (_evhttp != nullptr)
      evhttp_free(_evhttp);
  }
  void bind(const char *address, uint16_t port) {
    if (evhttp_bind_socket(_evhttp, address, port) < 0)
      throw RuntimeError("evhttp_bind_socket");
  }
  void set_allowed_methods(uint16_t methods) {  // e.g. EVHTTP_REQ_GET | EVHTTP_REQ_OPTIONS
    evhttp_set_allowed_methods(_evhttp, methods);
  }
  typedef std::function<void(HTTPRequest&&)> handler_t;
  void add(handler_t&& handler) {
    _handlers.emplace_back(std::move(handler));
    evhttp_set_gencb(_evhttp, callback, &*_handlers.rbegin());
  }
  void add(const char *path, handler_t&& handler) {
    _handlers.emplace_back(std::move(handler));
    if (evhttp_set_cb(_evhttp, path, callback, &*_handlers.rbegin()) < 0)
      throw RuntimeError("evhttp_set_cb");
  }

 private:
  static void callback(struct evhttp_request *evhttp_request, void *arg) {
    try {
      HTTPRequest request(evhttp_request);
      (*reinterpret_cast<handler_t *>(arg))(std::move(request));
    } catch (...) {
      unhandled_exception();
    }
  }

 private:
  HTTP() = delete;
  HTTP(const HTTP&) = delete;
  HTTP& operator=(const HTTP&) = delete;

 private:
  struct evhttp *_evhttp;
  std::list<handler_t> _handlers;
};

// TODO(thraneh): consider this pattern std::unique_ptr<evhttp, decltype(&evhttp_free)>(evhttp_start(...))

}  // namespace libevent
}  // namespace quinclas
