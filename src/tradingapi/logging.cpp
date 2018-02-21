/* Copyright (c) 2017-2018, Hans Erik Thrane */

#define UNW_LOCAL_ONLY

#include "quinclas/logging.h"

#include <libunwind.h>
#include <cxxabi.h>
#include <signal.h>

#if defined(__linux__)
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#include <libgen.h>
#include <sys/syslimits.h>
#endif

#include <cctz/time_zone.h>
// FIXME(thraneh): only do this when configure has detected spdlog
#include <spdlog/spdlog.h>

#include <chrono>
#include <fstream>
#include <iomanip>
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

#if !defined(QUINCLAS_GLOG)

static char proc_name[1024];
static const int width = (2 * sizeof(void *)) + 2;

static void print_stacktrace(int signal, siginfo_t *info) {
  psiginfo(info, nullptr);
  unw_context_t uc;
  if (unw_getcontext(&uc) != 0) {
    fprintf(stderr, "Unable to initialize libunwind context.\n");
    return;
  }
  unw_cursor_t cursor;
  if (unw_init_local(&cursor, &uc) < 0) {
    fprintf(stderr, "Unable to initialize libunwind cursor.\n");
    return;
  }
  int status;
  for (int index = 0;; ++index) {
    status = unw_step(&cursor);
    if (status == 0)  // done
      break;
    if (status < 0) {  // failure
      fprintf(stderr, "Unable to step libunwind cursor.\n");
      return;
    }
    unw_word_t ip = 0;
    if (unw_get_reg(&cursor, UNW_REG_IP, &ip) < 0) {
      fprintf(stderr, "Unable to get libunwind ip register.\n");
    }
    unw_word_t offp;
    status = unw_get_proc_name(&cursor, proc_name, sizeof(proc_name), &offp);
    const char *name = "<unknown>";
    char *demangled_name = nullptr;
    if (status < 0) {
      if (status != UNW_ENOINFO) {
        fprintf(stderr, "Unable to get libunwind proc_name.\n");
      }
    } else {
      name = proc_name;
      demangled_name = abi::__cxa_demangle(proc_name, nullptr, nullptr, &status);
      if (status == 0)
        name = demangled_name;
    }
    fprintf(stderr, "[%2d] %#*lx %s\n", index, width, ip, name);
    if (demangled_name)
      free(demangled_name);
  }
}

static void invoke_default_signal_handler(int signal) {
  struct sigaction sa = {};
  sigemptyset(&sa.sa_mask);
  sa.sa_handler = SIG_DFL;
  sigaction(signal, &sa, nullptr);
  kill(getpid(), signal);
}

static void termination_handler(int sig, siginfo_t *info, void *ucontext) {
  fprintf(stderr, "*** TERMINATION HANDLER ***\n");
  print_stacktrace(sig, info);
  invoke_default_signal_handler(sig);
}

#endif

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

#if !defined(QUINCLAS_GLOG)

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

void Logger::install_failure_signal_handler() {
  struct sigaction sa = {};
  sa.sa_sigaction = termination_handler;
  sa.sa_flags = SA_SIGINFO;
  sigaction(SIGABRT, &sa, nullptr);
  sigaction(SIGILL, &sa, nullptr);
  sigaction(SIGSEGV, &sa, nullptr);
}

#endif

}  // namespace logging
}  // namespace quinclas
