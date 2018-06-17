/* Copyright (c) 2017-2018, Hans Erik Thrane */

#define UNW_LOCAL_ONLY

#include "roq/logging.h"

#include <libunwind.h>
#include <cxxabi.h>
#include <signal.h>

// FIXME(thraneh): only do this when configure has detected spdlog
#include <spdlog/spdlog.h>

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "roq/platform.h"
#include "roq/stream.h"


namespace {

static roq::stream::details::TimePointStr<
  std::chrono::system_clock::time_point> get_date_time() {
  auto now = std::chrono::system_clock::now();
  return roq::stream::details::TimePointStr<decltype(now)>(now);
}

#if !defined(ROQ_GLOG)

static char proc_name[1024];
static const int width = (2 * sizeof(void *)) + 2;

static void print_stacktrace(int signal, siginfo_t *info) {
#if defined(__linux__)
  psiginfo(info, nullptr);
#endif
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
#if defined(__linux__)
    fprintf(stderr, "[%2d] %#*lx %s\n", index, width, ip, name);
#elif defined(__APPLE__)
    fprintf(stderr, "[%2d] %#*llx %s\n", index, width, ip, name);
#endif
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

namespace roq {
namespace logging {
namespace detail {

thread_local char message_buffer_raw[MESSAGE_BUFFER_SIZE];
thread_local char *message_buffer = message_buffer_raw;

bool newline = true;
uint32_t verbosity = 0;

spdlog::logger *spdlog_logger = nullptr;

}  // namespace detail

std::string Logger::get_argv0() {
  return platform::get_program();
}

#if !defined(ROQ_GLOG)

std::string Logger::get_filename() {
  auto log_dir = std::getenv("GLOG_log_dir");
  if (isatty(fileno(stdin)) && (log_dir == nullptr || strlen(log_dir) == 0))
      return "";
  auto program = platform::get_program();
  auto hostname = platform::get_hostname();
  auto username = platform::get_username();
  auto date_time = get_date_time();
  // glog uses <program>.<hostname>.<user>.log.<severity>.<date>.<time>.<pid>
  std::stringstream buffer;
  buffer << log_dir << "/" << program << "."
         << hostname << "." << username << ".log";
  auto base = buffer.str();
  buffer << "." << date_time.c_str() << "." << getpid();
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
}  // namespace roq
