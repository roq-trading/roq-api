/* Copyright (c) 2017-2019, Hans Erik Thrane */

#define UNW_LOCAL_ONLY

#include "roq/logging.h"

#include <unistd.h>

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>

#include <chrono>
#include <memory>

#include "roq/unwind.h"
#include "roq/memory.h"

namespace roq {

constexpr auto MESSAGE_BUFFER_SIZE = size_t{65536};

constexpr auto SPDLOG_QUEUE_SIZE = size_t{1024 * 1024};
constexpr auto SPDLOG_THREAD_COUNT = size_t{1};
constexpr auto SPDLOG_FLUSH_SECONDS = size_t{1};

namespace detail {
thread_local logging::page_aligned_vector<char> RAW_BUFFER(
    MESSAGE_BUFFER_SIZE);
thread_local std::pair<char *, size_t> message_buffer(
    &RAW_BUFFER[0],
    RAW_BUFFER.size());
}  // namespace detail

namespace {
static void invoke_default_signal_handler(int signal) {
  struct sigaction sa = {};
  sigemptyset(&sa.sa_mask);
  sa.sa_handler = SIG_DFL;
  sigaction(signal, &sa, nullptr);
  kill(getpid(), signal);
}
static void termination_handler(int sig, siginfo_t *info, void *) {
  fprintf(stderr, "*** TERMINATION HANDLER ***\n");
  unwind::print_stacktrace(sig, info);
  invoke_default_signal_handler(sig);
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

namespace {
static spdlog::logger *spdlog_logger = nullptr;
}  // namespace

namespace detail {
int verbosity = 0;
sink_t info = [](const std::string_view& message) {
  if (likely(spdlog_logger))
    spdlog_logger->info(message);
};
sink_t warning = [](const std::string_view& message) {
  if (likely(spdlog_logger))
    spdlog_logger->warn(message);
};
sink_t error = [](const std::string_view& message) {
  if (likely(spdlog_logger))
    spdlog_logger->error(message);
};
sink_t critical = [](const std::string_view& message) {
  if (likely(spdlog_logger)) {
    spdlog_logger->critical(message);
    spdlog_logger->flush();
  }
  std::abort();
};
}  // namespace detail

void Logger::initialize(bool stacktrace) {
  auto terminal = ::isatty(fileno(stdout));
  std::shared_ptr<spdlog::logger> logger;
  if (terminal) {
    logger = spdlog::stdout_logger_st("spdlog");
  } else {
    spdlog::init_thread_pool(SPDLOG_QUEUE_SIZE, SPDLOG_THREAD_COUNT);
    spdlog::flush_every(std::chrono::seconds(SPDLOG_FLUSH_SECONDS));
    logger = spdlog::stdout_logger_st<spdlog::async_factory>("spdlog");
  }
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
  spdlog_logger = logger.get();
  auto verbosity = std::getenv("ROQ_v");
  if (verbosity != nullptr && std::strlen(verbosity) > 0)
    detail::verbosity = std::atoi(verbosity);
  if (stacktrace)
    install_failure_signal_handler();
}

void Logger::shutdown() {
  // note! not thread-safe
  if (spdlog_logger) {
    spdlog_logger->flush();
    spdlog_logger = nullptr;
  }
  spdlog::drop_all();
}

}  // namespace roq
