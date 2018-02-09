/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

// Regardless of the logging framework, the exposed interface
// borrows heavily from what is used by glog and Easylogging++.

// glog (Google's logging framework)
#if defined(QUINCLAS_GLOG)
#include <glog/logging.h>

// spdlog (a lock-free asynchronous logging framework)
#elif defined(QUINCLAS_SPDLOG)
#include <spdlog/spdlog.h>

// no view on logging framework?
#else
#warning No specific logging framework has been selected.
#if !defined(QUINCLAS_STDLOG)
#define QUINCLAS_STDLOG
#endif
#endif

// Implement an interface supporting C++ streams.
#if !defined(QUINCLAS_GLOG)
#include <functional>
#include <cstring>
#include <iostream>
namespace quinclas {
namespace logging {
namespace implementation {
typedef std::function<void(const char *)> sink_t;
extern sink_t null_sink;
extern sink_t info_sink;
extern sink_t warning_sink;
extern sink_t error_sink;
extern sink_t fatal_sink;
#define RAW_LOG(logger, sink) logger(__FILE__, __LINE__, sink).stream()
#define LOG_INFO(logger) RAW_LOG(logger, ::quinclas::logging::info_sink)
#define LOG_WARNING(logger) RAW_LOG(logger, ::quinclas::logging::warning_sink)
#define LOG_ERROR(logger) RAW_LOG(logger, ::quinclas::logging::error_sink)
#define LOG_FATAL(logger) RAW_LOG(logger, ::quinclas::logging::fatal_sink)
#define LOG(level) LOG_ ## level(::quinclas::logging::implementation::LogMessage)
#define LOG_IF(level, condition) \
    !(condition) \
    ? (void)(0) \
    : ::quinclas::logging::implementation::LogMessageVoidify() & LOG(level)
#define PLOG(level) LOG_ ## level(::quinclas::logging::implementation::ErrnoLogMessage)
#if defined(NDEBUG)
#define DLOG(level) RAW_LOG(::quinclas::logging::implementation::NullStream, null_sink)
#else
#define DLOG(level) LOG(level)
#endif
thread_local char message_buffer[4096];
class LogMessage {
 public:
  LogMessage(const char *file, int line, sink_t sink)
      : _sink(sink), _stream(&message_buffer[0], sizeof(message_buffer)) {
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
    if (message_buffer[n - 1] != '\n')
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
class LogMessageVoidify {
 public:
  void operator&(std::ostream&) {}
};
}  // namespace implementation

}  // namespace logging
}  // namespace quinclas
#endif

namespace quinclas {
namespace logging {

class Logger {
 public:
  void initialize(int argc, char *argv[]);
};

}  // namespace logging
}  // namespace quinclas

#if !defined(QUINCLAS_GLOG)
template <typename T>
quinclas::logging::implementation::NullStream&
operator<<(quinclas::logging::implementation::NullStream& stream, T&) {
  return stream;
}
#endif
