/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <unistd.h>

#if defined(__APPLE__)
#include <mach-o/dyld.h>
#include <libgen.h>
#include <sys/syslimits.h>
#endif

#include <fstream>
#include <string>

namespace roq {
namespace platform {

inline pid_t get_pid() {
  return getpid();
}

inline std::string get_hostname() {
  char buffer[256];  // gethostname(2)
  if (gethostname(buffer, sizeof(buffer)) < 0)
    return "<hostname>";
  return buffer;
}

inline std::string get_username() {
  char buffer[33];  // SF294121
  if (getlogin_r(buffer, sizeof(buffer)) != 0)
    return "<username>";
  return buffer;
}

// TODO(thraneh): cross-platform, e.g. SO1023306
inline std::string get_program() {
#if defined(__linux__)
  std::ifstream comm("/proc/self/comm");
  std::string name;
  std::getline(comm, name);
  return name;
#elif defined(__APPLE__)
  char buffer[PATH_MAX];
  uint32_t size = sizeof(buffer);
  if (_NSGetExecutablePath(buffer, &size) == 0)
    return basename(buffer);
  else
    return "<program>";
#else
#error "Don't know how to find program name"
#endif
}

}  // namespace platform
}  // namespace roq
