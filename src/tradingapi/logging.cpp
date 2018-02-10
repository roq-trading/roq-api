/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "quinclas/logging.h"
#include <cctz/time_zone.h>
// FIXME(thraneh): only do this when configure has detected spdlog
#include <spdlog/spdlog.h>
#include <chrono>
#include <fstream>
#include <iostream>

namespace {
// TODO(thraneh): cross-platform, e.g. SO1023306
static std::string get_program() {
#if defined(__linux__)
  std::ifstream comm("/proc/self/comm");
  std::string name;
  std::getline(comm, name);
  return name;
#elif defined(__APPLE__)
  char buffer[1024];
  uint32_t size = sizeof(buffer);
  if (_NSGetExecutablePath(buffer, &size) == 0)
    return buffer;
  else
    return "<program>";
#else
#error "Don't know how to find program name"
#endif
}
static std::string get_hostname() {
  char buffer[256];  // gethostname(2)
  if (gethostname(buffer, sizeof(buffer)) < 0)
    return "<hostname>";
  return buffer;
}
static std::string get_username() {
  char buffer[33];  // SF294121
  if (getlogin_r(buffer, sizeof(buffer)) != 0)
    return "<username>";
  return buffer;
}
static std::string get_date_time() {
  return cctz::format(
      "%E4Y%m%d-%H%M%S",
      std::chrono::system_clock::now(),
      cctz::utc_time_zone());
}
}  // namespace

namespace quinclas {
namespace logging {
namespace detail {
thread_local char message_buffer_raw[MESSAGE_BUFFER_SIZE];
thread_local char *message_buffer = message_buffer_raw;
bool newline = true;
uint32_t verbosity = 0;
spdlog::logger *spdlog_logger = nullptr;
}  // namespace detail

std::string Logger::get_argv0() {
  return get_program();
}

std::string Logger::get_filename() {
  auto log_dir = std::getenv("GLOG_log_dir");
  if (isatty(fileno(stdin)) && (log_dir == nullptr || strlen(log_dir) == 0))
      return "";
  auto program = get_program();
  auto hostname = get_hostname();
  auto username = get_username();
  auto date_time = get_date_time();
  // glog uses <program>.<hostname>.<user>.log.<severity>.<date>.<time>.<pid>
  std::stringstream buffer;
  buffer << log_dir << "/" << program << "."
         << hostname << "." << username << ".log";
  auto base = buffer.str();
  buffer << "." << date_time << "." << getpid();
  auto result = buffer.str();
  // best effort
  unlink(base.c_str());
  if (symlink(result.c_str(), base.c_str()) < 0)
    std::cerr << "Warning! Unable to create symlink " << base.c_str() << std::endl;
  return result;
}
}  // namespace logging
}  // namespace quinclas
