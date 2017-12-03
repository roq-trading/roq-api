/* Copyright (c) 2017, Hans Erik Thrane */

#include "execution_engine/event.h"
#include <glog/logging.h>

namespace quinclas {
namespace event {

// Thread

void Thread::use() {
#if EVTHREAD_USE_PTHREADS_IMPLEMENTED == 1
    if (evthread_use_pthreads() < 0) {
        LOG(WARNING) << "evthread_use_pthreads() failed";
        throw std::runtime_error("unable to use pthreads");
    }
#else
#error "threading not supported"
#endif
}

// Base

Base::Base() :
    _base(event_base_new()) {
    if (_base == nullptr) {
        LOG(WARNING) << "event_base_new() failed";
        throw std::bad_alloc();
    }
}

Base::~Base() {
    if (_base != nullptr)
        event_base_free(_base);
}

void Base::dispatch() {
    int res = event_base_dispatch(_base);
    if (res < 0) {
        LOG(WARNING) << "event_base_dispatch() failed";
        throw std::runtime_error("event_base_dispatch() failed");
    }
}

void Base::loop(int flags) {
    int res = event_base_loop(_base, flags);
    if (res < 0) {
        LOG(WARNING) << "event_base_loop() failed";
        throw std::runtime_error("event_base_loop() failed");
    }
}

void Base::loopexit(const struct timeval *timeout) {
    int res = event_base_loopexit(_base, timeout);
    if (res < 0) {
        LOG(WARNING) << "event_base_loopexit() failed";
        throw std::runtime_error("event_base_loopexit() failed");
    }
}

void Base::loopbreak() {
    int res = event_base_loopbreak(_base);
    if (res < 0) {
        LOG(WARNING) << "event_base_loopbreak() failed";
        throw std::runtime_error("event_base_loopbreak() failed");
    }
}

// DNSBase

DNSBase::DNSBase(Base& base, int initialize_nameservers) :
    _evdns_base(evdns_base_new(base.get_raw(), initialize_nameservers)) {
    if (_evdns_base == nullptr) {
        LOG(WARNING) << "evdns_base_new() failed";
        throw std::bad_alloc();
    }
}

DNSBase::~DNSBase() {
    if (_evdns_base != nullptr) {
        int fail_requests = 1;
        evdns_base_free(_evdns_base, fail_requests);
    }
}

// Buffer

// TODO(thraneh): evbuffer_enable_locking
// TODO(thraneh): evbuffer_defer_callbacks ?

Buffer::Buffer() :
    _evbuffer(evbuffer_new()) {
    if (_evbuffer == nullptr) {
        LOG(WARNING) << "evbuffer_new() failed";
        throw std::bad_alloc();
    }
}

Buffer::~Buffer() {
    if (_evbuffer != nullptr)
        evbuffer_free(_evbuffer);
}

size_t Buffer::length() {
    return evbuffer_get_length(_evbuffer);
}

unsigned char *Buffer::pullup(ev_ssize_t size) {
    return evbuffer_pullup(_evbuffer, size);
}

void Buffer::drain(size_t length) {
    int res = evbuffer_drain(_evbuffer, length);
    if (res < 0) {
        LOG(WARNING) << "evbuffer_drain() failed";
        throw std::runtime_error("failed to drain the event buffer");
    }
}

void Buffer::add(const void *data, size_t datlen) {
    int res = evbuffer_add(_evbuffer, data, datlen);
    if (res < 0) {
        LOG(WARNING) << "evbuffer_add() failed";
        throw std::runtime_error("failed to add the event buffer");
    }
}

// Event

Event::Event(Base& base, int fd, short events, event_callback_fn callback, void *cbarg) :
    _event(event_new(base.get_raw(), fd, events, callback, cbarg)) {
    if (_event == nullptr) {
        LOG(WARNING) << "event_new() failed";
        throw std::bad_alloc();
    }
}

Event::~Event() {
    if (_event != nullptr)
        event_free(_event);
}

void Event::add(struct timeval *timeout) {
    if (event_add(_event, timeout) < 0) {
        LOG(WARNING) << "event_add() failed";
        throw std::runtime_error("Unable to add event");
    }
}

// TimerEvent

TimerEvent::TimerEvent(Handler& handler, Base& base) :
    Event(base, -1, EV_PERSIST, callback, &handler) {}

void TimerEvent::callback(evutil_socket_t, short, void *cbarg) {
    reinterpret_cast<Handler*>(cbarg)->on_timer();
}

// SignalEvent

SignalEvent::SignalEvent(Handler& handler, Base& base, int signal) :
    Event(base, signal, EV_SIGNAL|EV_PERSIST, callback, &handler) {}

void SignalEvent::callback(evutil_socket_t signal, short events, void *cbarg) {
    reinterpret_cast<Handler*>(cbarg)->on_signal(signal);
}

// BufferEvent

// TODO(thraneh): BEV_OPT_THREADSAFE ?

BufferEvent::BufferEvent(Base& base, net::Socket&& socket, int options) :
    _socket(std::move(socket)),
    _bufferevent(bufferevent_socket_new(base.get_raw(), _socket.get_raw(), options)) {
    if (_bufferevent == nullptr) {
        LOG(WARNING) << "bufferevent_socket_new() failed";
        throw std::bad_alloc();
    }
}

BufferEvent::~BufferEvent() {
    if (_bufferevent != nullptr)
        bufferevent_free(_bufferevent);
}

void BufferEvent::setcb(bufferevent_data_cb readcb, bufferevent_data_cb writecb, bufferevent_event_cb eventcb,
        void *cbarg) {
    bufferevent_setcb(_bufferevent, readcb, writecb, eventcb, cbarg);
}

void BufferEvent::enable(short events) {
    bufferevent_enable(_bufferevent, events);
}

void BufferEvent::read(Buffer& buffer) {
    int res = bufferevent_read_buffer(_bufferevent, buffer.get_raw());
    if (res < 0) {
        LOG(WARNING) << "bufferevent_read_buffer() failed";
        throw std::runtime_error("unable to read data");
    }
}

size_t BufferEvent::read(void* data, size_t length) {
    return bufferevent_read(_bufferevent, data, length);
}

void BufferEvent::write(const void* data, size_t length) {
    const int res = bufferevent_write(_bufferevent, data, length);
    if (res != 0) {
        LOG(ERROR) << "bufferevent_write() failed";
        throw std::runtime_error("unable to write");
    }
}

void BufferEvent::write(Buffer& buffer) {
    if (bufferevent_write_buffer(_bufferevent, buffer.get_raw()) < 0) {
        LOG(ERROR) << "bufferevent_write_buffer() failed";
        throw std::runtime_error("unable to write");
    }
}

void BufferEvent::connect(const struct sockaddr *address, int address_length) {
    if (bufferevent_socket_connect(_bufferevent, address, address_length) < 0) {
        LOG(ERROR) << "bufferevent_socket_connect() failed";
        throw std::runtime_error("unable to connect");
    }
}

void BufferEvent::connect(DNSBase& dns_base, int family, const char *hostname, int port) {
    if (bufferevent_socket_connect_hostname(_bufferevent, dns_base.get_raw(), family, hostname, port) < 0) {
        LOG(ERROR) << "bufferevent_socket_connect_hostname() failed";
        throw std::runtime_error("unable to connect");
    }
}

// Listener

Listener::Listener(Handler& handler, event::Base& base, int flags, int backlog, net::Socket&& socket) :
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

Listener::~Listener() {
    if (_evconnlistener != nullptr)
        evconnlistener_free(_evconnlistener);
}

void Listener::callback(struct evconnlistener *, evutil_socket_t fd, struct sockaddr *address,
        int address_length, void *cbarg) {
    auto& listener = *reinterpret_cast<Listener*>(cbarg);
    net::Socket socket(fd);
    DCHECK_EQ(socket.non_blocking(), true) << "Socket must be non-blocking";
    socket.no_delay(true);  // FIXME(thraneh): maybe not here?
    // LOG(INFO) << "Accepted new connection from " << net::Utilities::inet_ntop(AF_INET, &(address.sin_addr));
    event::BufferEvent buffer(listener._base, std::move(socket));
    listener._handler.on_accept(std::move(buffer));
}

}  // namespace event
}  // namespace quinclas
