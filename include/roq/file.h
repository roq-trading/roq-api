/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <string>
#include <system_error>

namespace roq {
namespace file {

// note! close to posix, *not* trying to mirror any c++ standard library

class File final {
 public:
  File(const char *path, int oflag, mode_t mode = 0)
      : _fd(::open(path, oflag, mode)) {
    if (_fd < 0)
      throw std::system_error(errno, std::system_category());
  }
  File(const std::string& path, int oflag, mode_t mode = 0)
      : File(path.c_str(), oflag) {}
  ~File() {
    if (_fd < 0)
      return;
    ::close(_fd);
  }
  int get() { return _fd; }
  struct stat stat() {
    struct stat stat;
    if (fstat(_fd, &stat) < 0)
      throw std::system_error(errno, std::system_category());
    return stat;
  }
  off_t size() {
    return stat().st_size;
  }
  void seek(off_t offset, int whence) {
    if (lseek(_fd, offset, whence) < 0)
      throw std::system_error(errno, std::system_category());
  }
  ssize_t write(const void *buf, size_t count) {
    ssize_t res = ::write(_fd, buf, count);
    if (res < 0)
      throw std::system_error(errno, std::system_category());
    return res;
  }
  // utilities
  void ensure_size(size_t length) {
    if (length <= size())
      return;
    // creates a hole (meaning: it doesn't allocate all inodes)
    seek(length - 1, SEEK_SET);
    write("", 1);
  }

 private:
  File(File&) = delete;
  File& operator=(File&) = delete;
  int _fd;
};

}  // namespace file
}  // namespace roq
