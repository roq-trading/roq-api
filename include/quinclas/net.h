/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <quinclas/logging.h>

#include <cstring>
#include <string>
#include <utility>

namespace quinclas {
namespace net {

class RuntimeError : public std::exception {
 public:
  explicit RuntimeError(std::string&& what) : _what(std::move(what)) {
    VLOG(1) << "RuntimeError, what=" << _what.c_str();
  }
  const char *what() const noexcept override { return _what.c_str(); }
 private:
  std::string _what;
};

class Address final {
 public:
  explicit Address(const char *path)
      : _size(sizeof(_address.un)) {
    std::memset(&_address.un, 0, sizeof(_address.un));
    _address.un.sun_family = AF_LOCAL;
    if (sizeof(_address.un.sun_path) <= std::strlen(path))
      throw RuntimeError("path is too long");
    strncpy(_address.un.sun_path, path, sizeof(_address.un.sun_path));
  }
  explicit Address(const std::string& path) : Address(path.c_str()) {}
  // use gethostbyname (or some similar async version) to look up the address given a hostname
  Address(const struct in_addr address, const uint16_t port)
      : _size(sizeof(_address.in)) {
    struct hostent *hostinfo;
    _address.in.sin_family = AF_INET;
    _address.in.sin_port = htons(port);
    _address.in.sin_addr = address;
  }
  const struct sockaddr *get() const {
    return reinterpret_cast<const struct sockaddr *>(&_address);
  }
  int get_family() const {
    return _address.sa.sa_family;
  }
  size_t size() const { return _size; }
  std::string to_string() const {
    switch (_size) {
      case sizeof(_address.un):
        return std::string(_address.un.sun_path);
      case sizeof(_address.in): {
        // TODO(thraneh): create ipv4 address by resolving hostname
        throw RuntimeError("ipv4 address not implemented");
      }
      default:
        assert(false);  // not a valid address?
    }
  }

 private:
  typedef union {
    struct sockaddr sa;
    struct sockaddr_in in;
    struct sockaddr_un un;
  } sockaddr_t;
  sockaddr_t _address;
  socklen_t _size;
};

class Socket final {
 public:
  explicit Socket(int fd) : _fd(fd) {}
  Socket(int domain, int type, int protocol) : _fd(socket(domain, type, protocol)) {
    if (_fd < 0) {
      PLOG(WARNING) << "socket() failed";
      throw std::system_error(errno, std::system_category());
    }
  }
  Socket(Socket&& rhs) : _fd(rhs._fd) {
    rhs._fd = -1;
  }
  ~Socket() {
    if (_fd >= 0)
      close(_fd);
  }
  Socket& operator=(Socket&& rhs) {
    if (_fd >= 0)
      close(_fd);
    _fd = rhs._fd;
    rhs._fd = -1;
    return *this;
  }
  operator int() const { return _fd; }
  int get() const { return _fd; }
  int release() {
    int res = -1;
    std::swap(_fd, res);
    return res;
  }
  void bind(const struct sockaddr *address, socklen_t address_len) {
    if (::bind(_fd, address, address_len) < 0) {
      PLOG(WARNING) << "bind() failed";
      throw std::system_error(errno, std::system_category());
    }
  }
  void bind(const Address& address) {
    bind(address.get(), address.size());
  }
  void listen(int backlog) {
    if (::listen(_fd, backlog) < 0) {
      PLOG(WARNING) << "bind() failed";
      throw std::system_error(errno, std::system_category());
    }
  }
  Socket accept(struct sockaddr *address, socklen_t *address_len) const {
    const auto fd = ::accept(_fd, address, address_len);
    if (fd < 0) {
      PLOG(WARNING) << "accept() failed";
      throw std::system_error(errno, std::system_category());
    }
    return Socket(fd);
  }
  int getfl() const {
    const auto flags = ::fcntl(_fd, F_GETFL);
    if (flags < 0) {
      PLOG(WARNING) << "fcntl(F_GETFL) failed";
      throw std::system_error(errno, std::system_category());
    }
    return flags;
  }
  void setfl(int option, bool mode) {
    auto flags = getfl();
    flags = mode ? (flags | option) : (flags & ~option);
    if (::fcntl(_fd, F_SETFL, flags) < 0) {
      PLOG(WARNING) << "fcntl(F_SETFL) failed";
      throw std::system_error(errno, std::system_category());
    }
  }
  template <typename T>
  T getsockopt(int level, int option_name) const {
    T value;
    socklen_t length = sizeof(value);
    if (::getsockopt(_fd, level, option_name, &value, &length) < 0) {
      PLOG(WARNING) << "getsockopt() failed";
      throw std::system_error(errno, std::system_category());
    }
    return value;
  }
  template <typename T>
  void setsockopt(int level, int option_name, T value) {
    if (::setsockopt(_fd, level, option_name, &value, sizeof(value)) < 0) {
      PLOG(WARNING) << "setsockopt() failed";
      throw std::system_error(errno, std::system_category());
    }
  }
  // non-standard helper functions
  bool non_blocking() const {
    return (getfl() & O_NONBLOCK) != 0;
  }
  void non_blocking(bool mode) {
    setfl(O_NONBLOCK, mode);
  }
  bool reuse_address() const {
    return getsockopt<int>(SOL_SOCKET, SO_REUSEADDR) != 0;
  }
  void reuse_address(bool mode) {
    const int value = mode ? 1 : 0;
    setsockopt(SOL_SOCKET, SO_REUSEADDR, value);
  }
  bool no_delay() const {
    return getsockopt<int>(IPPROTO_TCP, TCP_NODELAY) != 0;
  }
  void no_delay(bool mode) {
    const int value = mode ? 1 : 0;
    setsockopt(IPPROTO_TCP, TCP_NODELAY, value);
  }
  ssize_t write(const void *buf, size_t nbyte) {
    return ::write(_fd, buf, nbyte);
  }
  ssize_t writev(const struct iovec *iov, int iovcnt) {
    return ::writev(_fd, iov, iovcnt);
  }

 private:
  Socket() = delete;
  Socket(Socket const&) = delete;
  Socket& operator=(const Socket&) = delete;
  int _fd;
};

class SocketPair final {
 public:
  static std::pair<Socket, Socket> create(int type, int protocol = 0) {
    int fds[2];
    if (socketpair(PF_LOCAL, type, protocol, fds) < 0)
      throw std::system_error(errno, std::system_category());
    return std::make_pair(Socket(fds[0]), Socket(fds[1]));
  }
};

}  // namespace net
}  // namespace quinclas
