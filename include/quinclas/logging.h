/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

// Regardless of the logging framework, the interface exposed here
// borrows heavily from what is used by glog and Easylogging++.

#include <cassert>

// Define a default logger.
#if !defined(QUINCLAS_SPDLOG) && \
    !defined(QUINCLAS_STDLOG) && \
    !defined(QUINCLAS_GLOG)
#define QUINCLAS_SPDLOG
#endif

// Using glog (Google's logging framework).
#if defined(QUINCLAS_GLOG)
#include <glog/logging.h>

// Using spdlog (a lock-free asynchronous logging framework).
#elif defined(QUINCLAS_SPDLOG)
#include <spdlog/spdlog.h>

// Fallback to stdout/stderr.
#else
#if !defined(QUINCLAS_STDLOG)
#define QUINCLAS_STDLOG
#endif
#endif

// Always defined because we must support multiple logging backends.
namespace quinclas {
namespace logging {
namespace detail {
#define MESSAGE_BUFFER_SIZE 4096
extern thread_local char *message_buffer;
extern bool newline;
extern uint32_t verbosity;
}  // namespace detail
}  // namespace logging
}  // namespace quinclas

// Implement an interface supporting C++ streams.
#if !defined(QUINCLAS_GLOG)
#include <cstring>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
namespace quinclas {
namespace logging {
namespace detail {
typedef std::function<void(const char *)> sink_t;
// SO8487986
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define RAW_LOG(logger, sink) logger(__FILENAME__, __LINE__, sink).stream()
#if defined(QUINCLAS_SPDLOG)
extern spdlog::logger *spdlog_logger;
#define LOG_INFO(logger) RAW_LOG(logger, [](const char *message){ \
    ::quinclas::logging::detail::spdlog_logger->info(message); })
#define LOG_WARNING(logger) RAW_LOG(logger, [](const char *message){  \
    ::quinclas::logging::detail::spdlog_logger->warn(message); })
#define LOG_ERROR(logger) RAW_LOG(logger, [](const char *message){  \
    ::quinclas::logging::detail::spdlog_logger->error(message); })
#define LOG_FATAL(logger) RAW_LOG(logger, [](const char *message){  \
    ::quinclas::logging::detail::spdlog_logger->critical(message); \
    std::abort(); })
#else
#define LOG_INFO(logger) RAW_LOG(logger, [](const char *message){  \
    std::cout << "I " << message; })
#define LOG_WARNING(logger) RAW_LOG(logger, [](const char *message){  \
    std::cerr << "W " << message; })
#define LOG_ERROR(logger) RAW_LOG(logger, [](const char *message){  \
    std::cerr << "E " << message; })
#define LOG_FATAL(logger) RAW_LOG(logger, [](const char *message){  \
    std::cerr << "F " << message; std::abort(); })
#endif
#define LOG(level) LOG_ ## level(::quinclas::logging::detail::LogMessage)
#define LOG_IF(level, condition) \
    !(condition) \
    ? (void)(0) \
    : ::quinclas::logging::detail::LogMessageVoidify() & LOG(level)
#define PLOG(level) LOG_ ## level(::quinclas::logging::detail::ErrnoLogMessage)
#define VLOG(n) LOG_IF(INFO, (n)<=::quinclas::logging::detail::verbosity)
#if defined(NDEBUG)
#define DLOG(level) RAW_LOG(::quinclas::logging::detail::NullStream, [](const char * message){})
#else
#define DLOG(level) LOG(level)
#endif
class LogMessage {
 public:
  LogMessage(const char *file, int line, sink_t sink)
      : _sink(sink), _stream(&message_buffer[0], MESSAGE_BUFFER_SIZE) {
    _stream << file << ":" << line << "] ";
  }
  ~LogMessage() { flush(); }
  std::ostream& stream() { return _stream; }
  class LogStreamBuf : public std::streambuf {
   public:
    LogStreamBuf(char *buf, int len) {
      setp(buf, buf + len - 2);  // make space for "\n\0"
    }
    virtual int_type overflow(int_type ch) { return ch; }
    size_t pcount() const { return pptr() - pbase(); }
  };
  class LogStream : public std::ostream {
   public:
    LogStream(char *buf, int len) : std::ostream(nullptr), _streambuf(buf, len) {
      rdbuf(&_streambuf);
    }
    size_t pcount() const { return _streambuf.pcount(); }

   private:
    LogStreamBuf _streambuf;
  };

 private:
  void flush() {
    auto n = _stream.pcount();
    if (newline && message_buffer[n - 1] != '\n')
      message_buffer[n++] = '\n';
    message_buffer[n] = '\0';
    _sink(message_buffer);
  }
  sink_t _sink;
  LogStream _stream;
};
class ErrnoLogMessage : public LogMessage {
 public:
  ErrnoLogMessage(const char *file, int line, sink_t sink)
      : LogMessage(file, line, sink), _errnum(errno) {}
  ~ErrnoLogMessage() { flush(); }
 private:
  void flush() {
    stream() << ": " << std::strerror(_errnum) << " [" << _errnum << "]";
  }
  int _errnum;
};
class NullStream : public LogMessage::LogStream {
 public:
  NullStream(const char *file, int line, sink_t sink)
      : LogMessage::LogStream(_buffer, sizeof(_buffer)) {}
  NullStream& stream() { return *this; }
 private:
  char _buffer[2];
};
template <typename T>
quinclas::logging::detail::NullStream&
operator<<(NullStream& stream, T&) {
  return stream;
}
class LogMessageVoidify {
 public:
  void operator&(std::ostream&) {}
};
}  // namespace detail
}  // namespace logging
}  // namespace quinclas
#endif

namespace quinclas {
namespace logging {

class Logger {
 public:
  static void initialize(int argc, char *argv[]) {
#if defined(QUINCLAS_GLOG)
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();
#elif defined(QUINCLAS_SPDLOG)
    detail::newline = false;
    auto filename = get_filename();
    auto console = filename.empty();
    if (!console)
      spdlog::set_async_mode(
          8192,
          spdlog::async_overflow_policy::discard_log_msg,
          nullptr,
          std::chrono::seconds(3),
          nullptr);
    auto logger = console ?
                  spdlog::stdout_logger_st("spdlog") :
                  spdlog::basic_logger_st("spdlog", filename);
    logger->set_pattern("%L%m%d %T.%f %t %v");  // same as glog
    logger->flush_on(spdlog::level::warn);
    // note! spdlog keeps a reference
    ::quinclas::logging::detail::spdlog_logger = logger.get();
#else
#endif
  }
  static std::string get_filename();
};

}  // namespace logging
}  // namespace quinclas
