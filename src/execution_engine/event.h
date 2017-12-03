/* Copyright (c) 2017, Hans Erik Thrane */

#pragma once

#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/dns.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/thread.h>

#include "execution_engine/net.h"

namespace quinclas {
namespace event {

class Thread {
 public:
    static void use();
};

class Base {
 public:
    Base();
    Base(Base&& rhs) : _base(rhs._base) {}
    ~Base();
    operator struct event_base *() const { return _base; }
    struct event_base *get_raw() const { return _base; }
    void dispatch();
    void loop(int flags);
    void loopexit(const struct timeval *timeout = nullptr);
    void loopbreak();
 private:
    Base(Base const&) = delete;
    Base& operator=(Base const&) = delete;
    struct event_base *_base;
};

class DNSBase {
 public:
     DNSBase(Base& base, int initialize_nameservers);
     ~DNSBase();
    struct evdns_base *get_raw() { return _evdns_base; }
 private:
    struct evdns_base *_evdns_base;
};

class Buffer {
 public:
    Buffer();
    Buffer(Buffer&& rhs) : _evbuffer(std::move(rhs._evbuffer)) {}
    ~Buffer();
    struct evbuffer *get_raw() { return _evbuffer; }
    size_t length();
    unsigned char *pullup(ev_ssize_t size);
    void drain(size_t length);
    void add(const void *data, size_t datlen);
 private:
    Buffer(Buffer const&) = delete;
    Buffer& operator=(Buffer const&) = delete;
    struct evbuffer *_evbuffer;
};

class Event {
 public:
    Event(Base& base, int fd, short events, event_callback_fn callback, void *cbarg);
    ~Event();
    void add(struct timeval *timeout = nullptr);
 private:
    struct event *_event;
};

class TimerEvent : public Event {
 public:
    class Handler {
     public:
        virtual void on_timer() = 0;
    };
    TimerEvent(Handler& handler, Base& base);
 private:
    static void callback(evutil_socket_t, short, void *cbarg);
 private:
    TimerEvent() = delete;
    TimerEvent(TimerEvent const&) = delete;
    TimerEvent& operator=(TimerEvent const&) = delete;
};

class SignalEvent : public Event {
 public:
    class Handler {
     public:
        virtual void on_signal(int signal) = 0;
    };
    SignalEvent(Handler& handler, Base& base, int signal);
 private:
    static void callback(evutil_socket_t sig, short events, void *cbarg);
    SignalEvent() = delete;
    SignalEvent(SignalEvent const&) = delete;
    SignalEvent& operator=(SignalEvent const&) = delete;
};


class BufferEvent {
 public:
    BufferEvent(Base& base, DNSBase& dns_base, int family, const char *hostname, int port);
    BufferEvent(Base& base, net::Socket&& socket, int options = 0);
    BufferEvent(BufferEvent&& rhs) :
        _socket(std::move(rhs._socket)),
        _bufferevent(rhs._bufferevent) {}
    virtual ~BufferEvent();
    operator struct bufferevent *() { return _bufferevent; }
    struct bufferevent *get_raw() const { return _bufferevent; }
    void setcb(bufferevent_data_cb readcb, bufferevent_data_cb writecb, bufferevent_event_cb eventcb, void *cbarg);
    void enable(short events);
    void read(Buffer& buffer);
    size_t read(void* data, size_t length);
    void write(const void* data, size_t length);
    void write(Buffer& buffer);
    void connect(const struct sockaddr *address, int address_length);
    void connect(DNSBase& dns_base, int family, const char *hostname, int port);
 private:
    BufferEvent() = delete;
    BufferEvent(BufferEvent const&) = delete;
    BufferEvent& operator=(BufferEvent const&) = delete;
    net::Socket _socket;
    struct bufferevent *_bufferevent;
};

class Listener {
 public:
    class Handler {
     public:
        virtual void on_accept(event::BufferEvent&& buffer) = 0;
    };
    Listener(Handler& handler, event::Base& base, int flags, int backlog, net::Socket&& socket);
    ~Listener();
 private:
    static void callback(struct evconnlistener *, evutil_socket_t, struct sockaddr *, int socklen, void *);
 private:
    Listener() = delete;
    Listener(Listener const&) = delete;
    Listener& operator=(Listener const&) = delete;
    Handler& _handler;
    event::Base& _base;
    net::Socket _socket;
    struct evconnlistener *_evconnlistener;
};

}  // namespace event
}  // namespace quinclas
