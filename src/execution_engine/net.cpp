/* Copyright (c) 2017, Hans Erik Thrane */

#include "execution_engine/net.h"
#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <glog/logging.h>

namespace quinclas {
namespace net {

// Socket

Socket::Socket(int domain, int type, int protocol) :
    _fd(socket(domain, type, protocol)) {
    if (_fd < 0) {
        PLOG(WARNING) << "socket() failed";
        throw std::bad_alloc();
    }
}

Socket::~Socket() {
    if (_fd >= 0)
        close(_fd);
}

void Socket::bind(const struct sockaddr *address, socklen_t address_len) {
    if (::bind(_fd, address, address_len) < 0) {
        const std::error_code error_code(errno, std::system_category());
        PLOG(WARNING) << "bind() failed";
        throw std::system_error(error_code);
    }
}

void Socket::listen(int backlog) {
    if (::listen(_fd, backlog) < 0) {
        const std::error_code error_code(errno, std::system_category());
        PLOG(WARNING) << "bind() failed";
        throw std::system_error(error_code);
    }
}

Socket Socket::accept(struct sockaddr *address, socklen_t *address_len) {
    int res = ::accept(_fd, address, address_len);
    if (res < 0) {
        const std::error_code error_code(errno, std::system_category());
        PLOG(WARNING) << "accept() failed";
        throw std::system_error(error_code);
    }
    return Socket(res);
}

bool Socket::non_blocking() const {
    int flags = ::fcntl(_fd, F_GETFL);
    if (flags < 0) {
        const std::error_code error_code(errno, std::system_category());
        PLOG(WARNING) << "fcntl(F_SETFL) failed";
        throw std::system_error(error_code);
    }
    return (flags & O_NONBLOCK) != 0;
}

void Socket::non_blocking(bool mode) {
    int flags = ::fcntl(_fd, F_GETFL);
    if (flags < 0) {
        const std::error_code error_code(errno, std::system_category());
        PLOG(WARNING) << "fcntl(F_GETFL) failed";
        throw std::system_error(error_code);
    }
    if (mode)
        flags |= O_NONBLOCK;
    else
        flags &= ~O_NONBLOCK;
    if (::fcntl(_fd, F_SETFL, flags) < 0) {
        const std::error_code error_code(errno, std::system_category());
        PLOG(WARNING) << "fcntl(F_SETFL) failed";
        throw std::system_error(error_code);
    }
}

bool Socket::reuse_address() const {
    int value = 0;
    socklen_t length = sizeof(value);
    if (::getsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &value, &length) < 0) {
        const std::error_code error_code(errno, std::system_category());
        PLOG(WARNING) << "getsockopt(SOL_SOCKET, SO_REUSEADDR) failed";
        throw std::system_error(error_code);
    }
    return value != 0;
}

void Socket::reuse_address(bool mode) {
    const int value = mode ? 1 : 0;
    if (::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) < 0) {
        const std::error_code error_code(errno, std::system_category());
        PLOG(WARNING) << "setsockopt(SOL_SOCKET, SO_REUSEADDR) failed";
        throw std::system_error(error_code);
    }
}

bool Socket::no_delay() const {
    int value = 0;
    socklen_t length = sizeof(value);
    if (::getsockopt(_fd, IPPROTO_TCP, TCP_NODELAY, &value, &length) < 0) {
        const std::error_code error_code(errno, std::system_category());
        PLOG(WARNING) << "getsockopt(IPPROTO_TCP, TCP_NODELAY) failed";
        throw std::system_error(error_code);
    }
    return value != 0;
}

void Socket::no_delay(bool mode) {
    const int value = mode ? 1 : 0;
    if (::setsockopt(_fd, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value)) < 0) {
        const std::error_code error_code(errno, std::system_category());
        PLOG(WARNING) << "setsockopt(IPPROTO_TCP, TCP_NODELAY) failed";
        throw std::system_error(error_code);
    }
}

// Utilities

std::string Utilities::inet_ntop(int af, const void *src) {
    char buffer[INET_ADDRSTRLEN];
    ::inet_ntop(af, src, buffer, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';
    return buffer;
}

}  // namespace net
}  // namespace quinclas
