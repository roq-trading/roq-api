/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <cstdarg>
#include <string>

#include "roq/api.h"

namespace roq {

template <size_t N>
class BasicBufferWriter {
 public:
  explicit BasicBufferWriter(char *buffer)
      : _begin(buffer),
        _pointer(buffer) {
  }
  BasicBufferWriter& printf(const char *format, ...) {
    // TODO(thraneh): __attribute__((format(printf, 1, 2))) {
    va_list vargs;
    va_start(vargs, format);
    auto res = ::vsnprintf(_pointer, _length, format, vargs);
    va_end(vargs);
    advance(res);
    return *this;
  }
  BasicBufferWriter& strftime(const char *format, struct tm *tm) {
    auto res = ::strftime(_pointer, _length, format, tm);
    advance(res);
    return *this;
  }
  template <typename T>
  BasicBufferWriter& print(const T& value) {
    return write(*this, value);  // this function must exist
  }
  char *finish() {
    if (!_failed) {
      *_pointer = '\0';
      _pointer = nullptr;  // further actions will fail
      _length = 0;
      return _begin;
    } else {
      throw std::runtime_error("Print to buffer failed");
    }
  }
  void finish(std::string& string) {
    string.assign(finish());
  }
  std::ostream& finish(std::ostream& stream) {
    return stream << finish();
  }

 private:
  template <typename Res>
  void advance(Res res) {
    if (res > 0 && static_cast<size_t>(res) < _length) {
      _pointer += res;
      _length -= res;
    } else {
      _pointer = nullptr;
      _length = 0;
      _failed = true;
      throw std::runtime_error("overflow");
    }
  }

 private:
  char *_begin;
  char *_pointer;
  size_t _length = N;
  bool _failed = false;

  BasicBufferWriter(BasicBufferWriter&&) = delete;
  BasicBufferWriter(const BasicBufferWriter&) = delete;
  void operator=(const BasicBufferWriter&) = delete;
};

template <size_t N>
class StackWriter : public BasicBufferWriter<N> {
 public:
  StackWriter() : BasicBufferWriter<N>(_buffer) {}

 private:
  char _buffer[N];
};

}  // namespace roq
