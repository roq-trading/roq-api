/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <sys/mman.h>

#include "quinclas/file.h"

namespace quinclas {
namespace memory {

class Map final {
 public:
  Map(size_t length, int prot, int flags, int fd)
      : _length(length),
        _data(mmap(nullptr, _length, prot, flags, fd, 0)) {
    if (_data == MAP_FAILED)
      throw std::system_error(errno, std::system_category());
  }
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
  size_t _length;
  void *_data;
};

}  // namespace memory
}  // namespace quinclas
