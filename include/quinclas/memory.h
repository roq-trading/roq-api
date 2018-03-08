/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <sys/mman.h>
#include <fcntl.h>

#include "quinclas/file.h"

namespace quinclas {
namespace memory {

class Shared final {
 public:
  Shared(const char *name, int oflags, mode_t mode = 0)
      : _fd(shm_open(name, oflags, mode)) {
    if (_fd < 0)
      throw std::system_error(errno, std::system_category());
  }
  ~Shared() {
    if (_fd < 0)
      return;
    ::close(_fd);
  }
  int get() { return _fd; }
  void truncate(off_t length) {
    if (::ftruncate(_fd, length) < 0)
      throw std::system_error(errno, std::system_category());
  }

 private:
  Shared(Shared&) = delete;
  Shared& operator=(Shared&) = delete;
  int _fd;
};

class Map final {
 public:
  Map(size_t length, int prot, int flags, int fd)
      : _length(length),
        _data(mmap(nullptr, _length, prot, flags, fd, 0)) {
    if (_data == MAP_FAILED)
      throw std::system_error(errno, std::system_category());
  }
  Map(size_t length, int prot, int flags, Shared& shared)
      : Map(length, prot, flags, shared.get()) {}
  Map(size_t length, int prot, int flags, file::File& file)
      : Map(length, prot, flags, file.get()) {}
  ~Map() {
    if (_data == MAP_FAILED)
      return;
    munmap(_data, _length);
  }
  void *get() { return _data; }
  const void *get() const { return _data; }
  size_t length() const { return _length; }

 private:
  Map(Map&) = delete;
  Map& operator=(Map&) = delete;
  size_t _length;
  void *_data;
};

}  // namespace memory
}  // namespace quinclas
