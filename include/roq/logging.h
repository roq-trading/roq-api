/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <sstream>
#include <utility>

#include "roq/api.h"
#include "roq/static.h"

// The interface borrows heavily from glog and Easylogging++

namespace roq {

namespace detail {
extern ROQ_PUBLIC thread_local std::pair<char *, size_t> message_buffer;
extern ROQ_PUBLIC bool append_newline;
extern ROQ_PUBLIC int verbosity;
typedef std::function<void(const char *)> sink_t;
extern ROQ_PUBLIC sink_t info;
extern ROQ_PUBLIC sink_t warning;
extern ROQ_PUBLIC sink_t error;
extern ROQ_PUBLIC sink_t critical;
class ROQ_PUBLIC LogMessage {
 public:
  // stream buffer used for normal logging
  LogMessage(const char *prefix, const sink_t& sink)
      : _sink(sink),
        _stream(message_buffer.first, message_buffer.second) {
    _stream << prefix;
  }
  ~LogMessage() {
    flush();
  }
  std::ostream& stream() {
    return _stream;
  }
  class ROQ_PUBLIC LogStreamBuf : public std::streambuf {
   public:
    LogStreamBuf(char *buf, int len) {
      setp(buf, buf + len - 2);  // make space for "\n\0"
    }
    int_type overflow(int_type ch) override {
      return ch;
    }
    size_t pcount() const {
      return pptr() - pbase();
    }
  };  // class LogStreamBuf
  class ROQ_PUBLIC LogStream : public std::ostream {
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
    auto& buffer = message_buffer;
    if (append_newline && buffer.first[n - 1] != '\n')
      buffer.first[n++] = '\n';
    buffer.first[n] = '\0';
    _sink(buffer.first);
  }
  const sink_t& _sink;
  LogStream _stream;
};  // class LogMessage
// specialised stream buffer used for errno logging
class ROQ_PUBLIC ErrnoLogMessage : public LogMessage {
 public:
  ErrnoLogMessage(const char *prefix, const sink_t& sink)
      : LogMessage(prefix, sink),
        _errnum(errno) {
  }
  ~ErrnoLogMessage() {
    flush();  // remember: not a virtual base
  }
 private:
  void flush() {
    stream() << ": " << std::strerror(_errnum) << " [" << _errnum << "]";
  }
  int _errnum;
};  // class ErrnoLogMessage
class ROQ_PUBLIC NullStream : public LogMessage::LogStream {
 public:
  NullStream(const char *, const sink_t&)
      : LogMessage::LogStream(_buffer, sizeof(_buffer)) {
  }
  NullStream& stream() {
    return *this;
  }
 private:
  char _buffer[2];
};  // class NullStream
template <typename T>
roq::detail::NullStream&
operator<<(NullStream& stream, T&) {
  return stream;
}
class ROQ_PUBLIC LogMessageVoidify {
 public:
  void operator&(std::ostream&) {}
};  // class LogMessageVoidify
}  // namespace detail

struct ROQ_PUBLIC Logger final {
  static void initialize(bool stacktrace = true);
  static void shutdown();
};

}  // namespace roq

// Convert number to string (SO2670816)
#define STRINGIFY(number) STRINGIFY2(number)
#define STRINGIFY2(number) #number

// Raw logging itnerface
#define RAW_LOG(logger, sink) \
  logger( \
      roq::static_basename_string(__FILE__).append( \
          roq::static_string(":" STRINGIFY(__LINE__) "] ")).data(), sink).stream()

// Sink selectors
#define LOG_INFO(logger) RAW_LOG(logger, ::roq::detail::info)
#define LOG_WARNING(logger) RAW_LOG(logger, ::roq::detail::warning)
#define LOG_ERROR(logger) RAW_LOG(logger, ::roq::detail::error)
// FIXME(thraneh): SO26888805 [[noreturn]]
#define LOG_FATAL(logger) RAW_LOG(logger, ::roq::detail::critical)

// The main logging interface (level in {INFO|WARNING|ERROR|FATAL})
#define LOG(level) LOG_ ## level(::roq::detail::LogMessage)

// Conditional logging
#define LOG_IF(level, condition) \
    ROQ_LIKELY(!(condition)) \
    ? (void)(0) \
    : ::roq::detail::LogMessageVoidify() & LOG(level)

// System error logging
#define PLOG(level) LOG_ ## level(::roq::detail::ErrnoLogMessage)

// Verbose logging
#define VLOG(n) LOG_IF(INFO, (n) <= ::roq::detail::verbosity)

// Debug logging
#if defined(NDEBUG)
#define DLOG(level) LOG_ ## level(::roq::detail::NullStream)
#else
#define DLOG(level) LOG(level)
#endif
