/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <chrono>

#include "roq/print.h"

namespace roq {
namespace stream {

// BasicTimePointStr
class BasicTimePointStr : NonCopyable {
 public:
  explicit BasicTimePointStr(std::chrono::nanoseconds nanoseconds);
  const char *c_str() const {
    return _result;
  }

 private:
  char _result[30];  // YYYY-MM-DDTHH:MM:SS.NNNNNNNNN\0
};  // BasicTimePointStr

// TimePointStr
template <typename T>
class TimePointStr final : public BasicTimePointStr {
 public:
  explicit TimePointStr(T duration)
      : BasicTimePointStr(
            std::chrono::duration_cast<std::chrono::nanoseconds>(duration)) {
  }
};  // TimePointStr

// BasicClockStr
class BasicClockStr : NonCopyable {
 public:
  explicit BasicClockStr(std::chrono::nanoseconds nanoseconds);
  const char *c_str() const {
    return _result;
  }

 private:
  char _result[21];  // SSSSSSSSSS.NNNNNNNNN\0
};  // BasicClockStr

// ClockStr
template <typename T>
class ClockStr final : public BasicClockStr {
 public:
  explicit ClockStr(T duration)
      : BasicClockStr(
            std::chrono::duration_cast<std::chrono::nanoseconds>(duration)) {
  }
};  // ClockStr

}  // namespace stream
}  // namespace roq
