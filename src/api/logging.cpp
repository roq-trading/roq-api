/* Copyright (c) 2017-2018, Hans Erik Thrane */

#define UNW_LOCAL_ONLY

#include "roq/logging.h"

#include <libunwind.h>
#include <cxxabi.h>
#include <signal.h>

#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "roq/platform.h"
#include "roq/stream.h"

#define QUEUE_SIZE 1024*1024
#define THREAD_COUNT 1
#define FLUSH_SECONDS 3

namespace {

static roq::stream::details::TimePointStr<
  std::chrono::system_clock::time_point> get_date_time() {
  auto now = std::chrono::system_clock::now();
  return roq::stream::details::TimePointStr<decltype(now)>(now);
}

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

static std::string get_argv0() {
  return roq::platform::get_program();
}

static std::string get_filename(const char *log_dir) {
  if (log_dir == nullptr) {
    log_dir = std::getenv("ROQ_log_dir");
    if (log_dir == nullptr)
      log_dir = std::getenv("GLOG_log_dir");  // backwards compatibility
    if (isatty(fileno(stdin)) &&
        (log_dir == nullptr || strlen(log_dir) == 0))
        return "";
  }
  auto program = roq::platform::get_program();
  auto hostname = roq::platform::get_hostname();
  auto username = roq::platform::get_username();
  // FIXME(thraneh): we need a generic non-optimized date-time formatter
  std::string date_time = get_date_time().c_str();
  date_time.erase(
      std::remove(date_time.begin(), date_time.end(), '-'),
      date_time.end());
  date_time.erase(
      std::remove(date_time.begin(), date_time.end(), ':'),
      date_time.end());
  std::replace(date_time.begin(), date_time.end(), 'T', '-');
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

static void install_failure_signal_handler() {
  struct sigaction sa = {};
  sa.sa_sigaction = termination_handler;
  sa.sa_flags = SA_SIGINFO;
  sigaction(SIGABRT, &sa, nullptr);
  sigaction(SIGILL, &sa, nullptr);
  sigaction(SIGSEGV, &sa, nullptr);
}

}  // namespace

namespace roq {
namespace logging {
namespace detail {

thread_local char message_buffer_raw[MESSAGE_BUFFER_SIZE];
thread_local char *message_buffer = message_buffer_raw;

bool append_newline = true;
int verbosity = 0;

spdlog::logger *spdlog_logger = nullptr;

}  // namespace detail

void Logger::initialize(bool stacktrace, const char *log_dir) {
  detail::append_newline = false;
  auto filename = get_filename(log_dir);
  auto console = filename.empty();
  spdlog::init_thread_pool(QUEUE_SIZE, THREAD_COUNT);
  spdlog::flush_every(std::chrono::seconds(FLUSH_SECONDS));
  auto logger = console ?
      spdlog::stdout_logger_st("spdlog") :
      spdlog::basic_logger_st<spdlog::async_factory>(
          "spdlog", filename);
  // matching spdlog pattern to glog
  // - %L = level (I=INFO|W=WARN|E=ERROR|C=CRITICAL)
  // - %m = month (MM)
  // - %d = day (DD)
  // - %T = time (HH:MM:SS)
  // - %f = fraction (microseconds)
  // - %t = thread (int)
  // - %v = message
  logger->set_pattern("%L%m%d %T.%f %t %v");
  logger->flush_on(spdlog::level::warn);
  // note! spdlog uses reference count
  ::roq::logging::detail::spdlog_logger = logger.get();
  auto verbosity = std::getenv("ROQ_v");
  if (verbosity == nullptr)
    verbosity = std::getenv("GLOG_v");  // backwards compatibility
  if (verbosity != nullptr && strlen(verbosity) > 0)
    detail::verbosity = std::atoi(verbosity);
  if (stacktrace)
    install_failure_signal_handler();
}

void Logger::shutdown() {
  // note! not thread-safe
  if (::roq::logging::detail::spdlog_logger) {
    ::roq::logging::detail::spdlog_logger->flush();
    ::roq::logging::detail::spdlog_logger = nullptr;
  }
  spdlog::drop_all();
}

}  // namespace logging
}  // namespace roq
