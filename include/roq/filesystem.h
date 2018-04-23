/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <fcntl.h>
#include <libgen.h>
#include <unistd.h>

#include <sys/param.h>

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
inline std::string dirname(const char *path) {
  std::string buffer(path);
  auto result = ::dirname(&buffer.at(0));
  return std::string(result);
}
inline std::string basename(const char *path) {
#ifdef __APPLE__
  auto result = ::basename(path);
#else
  std::string buffer(path);
  auto result = ::basename(&buffer.at(0));
#endif
  if (result == nullptr)
    throw std::system_error(errno, std::system_category());
  return std::string(result);
}
inline std::string getcwd() {
  char buffer[MAXPATHLEN];
  auto result = ::getcwd(buffer, sizeof(buffer));
  if (result == nullptr)
    throw std::system_error(errno, std::system_category());
  return std::string(result);
}
inline void chdir(const char *path) {
  if (::chdir(path) != 0)
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

// dirname

inline std::string dirname(const char *path) {
  return detail::dirname(path);
}

inline std::string dirname(const std::string& path) {
  return dirname(path.c_str());
}

// basename

inline std::string basename(const char *path) {
  return detail::basename(path);
}

inline std::string basename(const std::string& path) {
  return basename(path.c_str());
}

// getcwd

inline std::string getcwd() {
  return detail::getcwd();
}

// chdir

inline void chdir(const char *path) {
  detail::chdir(path);
}

inline void chdir(const std::string& path) {
  chdir(path.c_str());
}

}  // namespace filesystem
}  // namespace roq
