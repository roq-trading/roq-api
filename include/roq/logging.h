/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

// The interface exposed here borrows heavily from that implemented
// by glog and Easylogging++.

// Using spdlog (a lock-free asynchronous logging framework).
#include <spdlog/spdlog.h>

// Always defined because we must support multiple logging backends.
namespace roq {
namespace logging {
namespace detail {
#define MESSAGE_BUFFER_SIZE 4096
extern thread_local char *message_buffer;
extern bool append_newline;
extern int verbosity;
}  // namespace detail
}  // namespace logging
}  // namespace roq

// Implement an interface supporting C++ streams.
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>
namespace roq {
namespace logging {
namespace detail {
typedef std::function<void(const char *)> sink_t;
// SO2670816
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
// FIXME(thraneh): truncate __FILE__ at compile-time
#define RAW_LOG(logger, sink) \
    logger(__FILE__, ":" STRINGIZE(__LINE__) "] ", sink).stream()
// spdlog output
extern spdlog::logger *spdlog_logger;
#define LOG_INFO(logger) RAW_LOG(logger, [](const char *message){ \
    ::roq::logging::detail::spdlog_logger->info(message); })
#define LOG_WARNING(logger) RAW_LOG(logger, [](const char *message){  \
    ::roq::logging::detail::spdlog_logger->warn(message); })
#define LOG_ERROR(logger) RAW_LOG(logger, [](const char *message){  \
    ::roq::logging::detail::spdlog_logger->error(message); })
// FIXME(thraneh): SO26888805 [[noreturn]]
#define LOG_FATAL(logger) RAW_LOG(logger, [](const char *message){  \
    ::roq::logging::detail::spdlog_logger->critical(message); \
    ::roq::logging::detail::spdlog_logger->flush(); \
    std::abort(); })
#define LOG(level) LOG_ ## level(::roq::logging::detail::LogMessage)
#define LOG_IF(level, condition) \
    !(condition) \
    ? (void)(0) \
    : ::roq::logging::detail::LogMessageVoidify() & LOG(level)
#define PLOG(level) LOG_ ## level(::roq::logging::detail::ErrnoLogMessage)
#define VLOG(n) LOG_IF(INFO, (n) <= ::roq::logging::detail::verbosity)
#if defined(NDEBUG)
#define DLOG(level) RAW_LOG(::roq::logging::detail::NullStream, [](const char * message){})
#else
#define DLOG(level) LOG(level)
#endif
class LogMessage {
 public:
  // stream buffer used for normal logging
  // TODO(thraneh): deprecate this version when filename can be found at compile-time
  LogMessage(const char *file, const char *prefix, sink_t sink)
      : _sink(sink),
        _stream(&message_buffer[0], MESSAGE_BUFFER_SIZE) {
    // FIXME(thraneh): can't we do this at compile-time ??? (SO8487986)
    auto last_sep = strrchr(file, '/');
    auto filename = last_sep ? (last_sep + 1) : file;
    _stream << filename << prefix;
  }
  LogMessage(const char *prefix, sink_t sink)
      : _sink(sink),
        _stream(&message_buffer[0], MESSAGE_BUFFER_SIZE) {
    _stream << prefix;
  }
  ~LogMessage() {
    flush();
  }
  std::ostream& stream() {
    return _stream;
  }
  class LogStreamBuf : public std::streambuf {
   public:
    LogStreamBuf(char *buf, int len) {
      setp(buf, buf + len - 2);  // make space for "\n\0"
    }
    virtual int_type overflow(int_type ch) {
      return ch;
    }
    size_t pcount() const {
      return pptr() - pbase();
    }
  };  // class LogStreamBuf
  class LogStream : public std::ostream {
   public:
    LogStream(char *buf, int len)
        : std::ostream(nullptr),
        _streambuf(buf, len) {
      rdbuf(&_streambuf);
    }
    size_t pcount() const {
      return _streambuf.pcount();
    }

   private:
    LogStreamBuf _streambuf;
  };  // class LogStream

 private:
  void flush() {
    auto n = _stream.pcount();
    if (append_newline && message_buffer[n - 1] != '\n')
      message_buffer[n++] = '\n';
    message_buffer[n] = '\0';
    _sink(message_buffer);
  }
  sink_t _sink;
  LogStream _stream;
};  // class LogMessage
// specialised stream buffer used for errno logging
class ErrnoLogMessage : public LogMessage {
 public:
  ErrnoLogMessage(const char *file, const char *prefix, sink_t sink)
      : LogMessage(file, prefix, sink),
        _errnum(errno) {
  }
  ErrnoLogMessage(const char *prefix, sink_t sink)
      : LogMessage(prefix, sink),
        _errnum(errno) {
  }
  ~ErrnoLogMessage() {
    flush();
  }
 private:
  void flush() {
    stream() << ": " << std::strerror(_errnum) << " [" << _errnum << "]";
  }
  int _errnum;
};  // class ErrnoLogMessage
class NullStream : public LogMessage::LogStream {
 public:
  NullStream(const char *file, const char *prefix, sink_t sink)
      : LogMessage::LogStream(_buffer, sizeof(_buffer)) {
  }
  NullStream(const char *prefix, sink_t sink)
      : LogMessage::LogStream(_buffer, sizeof(_buffer)) {
  }
  NullStream& stream() {
    return *this;
  }
 private:
  char _buffer[2];
};  // class NullStream
template <typename T>
roq::logging::detail::NullStream&
operator<<(NullStream& stream, T&) {
  return stream;
}
class LogMessageVoidify {
 public:
  void operator&(std::ostream&) {}
};  // class LogMessageVoidify
}  // namespace detail
}  // namespace logging
}  // namespace roq

namespace roq {
namespace logging {

class Logger {
 public:
  static void initialize(
      bool stacktrace = true,
      const char *log_dir = nullptr);
  static void shutdown();
};

}  // namespace logging
}  // namespace roq
