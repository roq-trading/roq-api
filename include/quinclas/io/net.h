/* Copyright (c) 2017, Hans Erik Thrane */

#pragma once

#include <fcntl.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <glog/logging.h>

#include <string>

namespace quinclas {
namespace io {
namespace net {

class Socket {
 public:
  explicit Socket(int fd) : _fd(fd) {}
  Socket(int domain, int type, int protocol) :
    _fd(socket(domain, type, protocol)) {
    if (_fd < 0) {
      PLOG(WARNING) << "socket() failed";
      throw std::bad_alloc();
    }
  }
  Socket(Socket&& rhs) : _fd(rhs._fd) {}
  ~Socket() {
    if (_fd >= 0)
      close(_fd);
  }
  operator int() const { return _fd; }
  int get_raw() const { return _fd; }
  void bind(const struct sockaddr *address, socklen_t address_len) {
    if (::bind(_fd, address, address_len) < 0) {
      const std::error_code error_code(errno, std::system_category());
      PLOG(WARNING) << "bind() failed";
      throw std::system_error(error_code);
    }
  }
  template <typename T>
  void bind(const T& address) {
    if (::bind(_fd, address.raw(), address.size()) < 0) {
      const std::error_code error_code(errno, std::system_category());
      PLOG(WARNING) << "bind() failed";
      throw std::system_error(error_code);
    }
  }
  void listen(int backlog) {
    if (::listen(_fd, backlog) < 0) {
      const std::error_code error_code(errno, std::system_category());
      PLOG(WARNING) << "bind() failed";
      throw std::system_error(error_code);
    }
  }
  Socket accept(struct sockaddr *address, socklen_t *address_len) {
    const auto fd = ::accept(_fd, address, address_len);
    if (fd < 0) {
      const std::error_code error_code(errno, std::system_category());
      PLOG(WARNING) << "accept() failed";
      throw std::system_error(error_code);
    }
    return Socket(fd);
  }
  // non-standard
  bool non_blocking() const {
    const auto flags = ::fcntl(_fd, F_GETFL);
    if (flags < 0) {
      const std::error_code error_code(errno, std::system_category());
      PLOG(WARNING) << "fcntl(F_SETFL) failed";
      throw std::system_error(error_code);
    }
    return (flags & O_NONBLOCK) != 0;
  }
  void non_blocking(bool mode) {
    auto flags = ::fcntl(_fd, F_GETFL);
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
  bool reuse_address() const {
    int value = 0;
    socklen_t length = sizeof(value);
    if (::getsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &value, &length) < 0) {
      const std::error_code error_code(errno, std::system_category());
      PLOG(WARNING) << "getsockopt(SOL_SOCKET, SO_REUSEADDR) failed";
      throw std::system_error(error_code);
    }
    return value != 0;
  }
  void reuse_address(bool mode) {
    const int value = mode ? 1 : 0;
    if (::setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)) < 0) {
      const std::error_code error_code(errno, std::system_category());
      PLOG(WARNING) << "setsockopt(SOL_SOCKET, SO_REUSEADDR) failed";
      throw std::system_error(error_code);
    }
  }
  bool no_delay() const {
    int value = 0;
    socklen_t length = sizeof(value);
    if (::getsockopt(_fd, IPPROTO_TCP, TCP_NODELAY, &value, &length) < 0) {
      const std::error_code error_code(errno, std::system_category());
      PLOG(WARNING) << "getsockopt(IPPROTO_TCP, TCP_NODELAY) failed";
      throw std::system_error(error_code);
    }
    return value != 0;
  }
  void no_delay(bool mode) {
    const int value = mode ? 1 : 0;
    if (::setsockopt(_fd, IPPROTO_TCP, TCP_NODELAY, &value, sizeof(value)) < 0) {
      const std::error_code error_code(errno, std::system_category());
      PLOG(WARNING) << "setsockopt(IPPROTO_TCP, TCP_NODELAY) failed";
      throw std::system_error(error_code);
    }
  }

 private:
  Socket() = delete;
  Socket(Socket const&) = delete;
  Socket& operator=(const Socket&) = delete;
  int _fd;
};

class UnixAddress {
 public:
  explicit UnixAddress(const char *path) {
    _address.sun_family = AF_LOCAL;
    strncpy(_address.sun_path, path, sizeof(_address.sun_path));
    _address.sun_path[sizeof(_address.sun_path) - 1] = '\0';
  }
  size_t size() const { return sizeof(_address); }
  const struct sockaddr *raw() const {
    return reinterpret_cast<const struct sockaddr *>(&_address);
  }
 private:
  struct sockaddr_un _address;
};

}  // namespace net
}  // namespace io
}  // namespace quinclas
