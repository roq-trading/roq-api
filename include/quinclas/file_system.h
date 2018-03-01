/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <fcntl.h>
#include <unistd.h>

#include <string>
#include <system_error>

namespace quinclas {
namespace file_system {

// note! many of the functions implemented here use exceptions
// for control-flow -- you should probably choose other options
// if you want such functionality in a critical code-path

class File {
 public:
  // access
  static void access(const char *path, int mode) {
    if (::access(path, mode) != 0)
      throw std::system_error(errno, std::system_category());
  }
  static void access(const std::string& path, int mode) {
    File::access(path.c_str(), mode);
  }
  // unlink
  static void unlink(const char *path) {
    if (::unlink(path) != 0)
      throw std::system_error(errno, std::system_category());
  }
  static void unlink(const std::string& path) {
    File::unlink(path.c_str());
  }
  // non-standard
  // exists
  static bool exists(const char *path) {
    try {
      File::access(path, F_OK);
      return true;
    } catch (std::system_error& e) {
      return false;
    }
  }
  static bool exists(const std::string& path) {
    return File::exists(path.c_str());
  }
  // can_write
  static bool can_write(const char *path) {
    try {
      File::access(path, W_OK);
    } catch (std::system_error& e) {
      return false;
    }
  }
  static bool can_write(const std::string& path) {
    return File::can_write(path.c_str());
  }
};

}  // namespace file_system
}  // namespace quinclas
