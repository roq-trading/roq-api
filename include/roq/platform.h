/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <unistd.h>

#if defined(__linux__)
#include <sys/types.h>
#include <pwd.h>
#endif

#if defined(__APPLE__)
#include <mach-o/dyld.h>
#include <libgen.h>
#include <sys/syslimits.h>
#endif

#include <fstream>
#include <string>
#include <vector>

namespace roq {
namespace platform {

inline pid_t get_pid() {
  return getpid();
}

inline std::string get_hostname() {
  char buffer[256];  // gethostname(2)
  if (gethostname(buffer, sizeof(buffer)) >= 0)
    return buffer;
  return "<hostname?>";
}

inline std::string get_username() {
  char buffer[33];  // SF294121
  if (getlogin_r(buffer, sizeof(buffer)) == 0)
    return buffer;
#if defined(__linux__)
  auto size = sysconf(_SC_GETPW_R_SIZE_MAX);
  std::vector<char> buf(size > 0 ? size : 16384);
  struct passwd pwd;
  struct passwd *result;
  if (getpwuid_r(getuid(), &pwd, &buf[0], buf.size(), &result) == 0)
    return pwd.pw_name;
#endif
  return "<username?>";
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
  return "<program?>";
#else
#error "Don't know how to find program name"
#endif
}

inline size_t get_page_size() {
  return getpagesize();
}

}  // namespace platform
}  // namespace roq
