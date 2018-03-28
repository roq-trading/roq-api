/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <fcntl.h>
#include <unistd.h>

#include <string>
#include <system_error>

namespace roq {
namespace filesystem {

// note! many of the functions implemented here use exceptions
// for control-flow -- you should probably choose other options
// if you want such functionality in a critical code-path
// this will eventually dropped in favour of the c++17 solution

namespace detail {
inline void access(const char *path, int mode) {
  if (::access(path, mode) != 0)
    throw std::system_error(errno, std::system_category());
}
inline void remove(const char *path) {
  if (::unlink(path) != 0)
    throw std::system_error(errno, std::system_category());
}
}  // namespace detail

// exists

inline bool exists(const char *path) {
  try {
    detail::access(path, F_OK);
    return true;
  } catch (std::system_error& e) {
    return false;
  }
}

inline bool exists(const std::string& path) {
  return exists(path.c_str());
}

// can_write (non-standard)

inline bool can_write(const char *path) {
  try {
    detail::access(path, W_OK);
    return true;
  } catch (std::system_error& e) {
    return false;
  }
}

inline bool can_write(const std::string& path) {
  return can_write(path.c_str());
}

// remove

inline void remove(const char *path) {
  detail::remove(path);
}

inline void remove(const std::string& path) {
  detail::remove(path.c_str());
}

}  // namespace filesystem
}  // namespace roq
