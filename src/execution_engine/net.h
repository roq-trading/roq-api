/* Copyright (c) 2017, Hans Erik Thrane */

#pragma once

#include <sys/socket.h>
#include <string>

namespace quinclas {
namespace net {

class Socket {
 public:
    explicit Socket(int fd) : _fd(fd) {}
    Socket(int domain, int type, int protocol);
    Socket(Socket&& rhs) : _fd(rhs._fd) {}
    ~Socket();
    operator int() const { return _fd; }
    int get_raw() const { return _fd; }
    void bind(const struct sockaddr *address, socklen_t address_len);
    void listen(int backlog);
    Socket accept(struct sockaddr *address, socklen_t *address_len);
    // non-standard
    bool non_blocking() const;
    void non_blocking(bool mode);
    bool reuse_address() const;
    void reuse_address(bool mode);
    bool no_delay() const;
    void no_delay(bool mode);
 private:
    Socket() = delete;
    Socket(Socket const&) = delete;
    Socket& operator=(const Socket&) = delete;
    int _fd;
};

class Utilities {
 public:
    static std::string inet_ntop(int af, const void *src);
};

}  // namespace net
}  // namespace quinclas
