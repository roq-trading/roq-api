/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include "roq/stream/utility.h"

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include <string>
#include <unordered_map>

#define STACK_BUFFER_SIZE 4096
#define FLOAT_REPR ".8g"  // include 1/256th
#define PASSWORD "<hidden>"

namespace roq {
namespace stream {

/*
// FIXME(thraneh): this only works if we drop thread_local from cached_time_str
// --> in which case it's too dangerous to explicitly call warm_cache()
void warm_cache() {
  auto time_point = std::chrono::system_clock::from_time_t(0);
  auto one_second = std::chrono::seconds(1);
  for (auto i = 0; i < 86400; ++i, time_point += one_second) {
    stream::TimePointStr<decltype(time_point)>(time_point).c_str();
  }
}
*/

// cache

thread_local std::unordered_map<uint32_t, std::string> cached_date_str;
thread_local std::unordered_map<uint32_t, std::string> cached_time_str;

// BasicTimePointStr

BasicTimePointStr::BasicTimePointStr(std::chrono::nanoseconds nanoseconds) {
  auto count = nanoseconds.count();
  auto seconds = static_cast<uint32_t>(count / UINT64_C(1000000000));
  auto fraction = count % UINT64_C(1000000000);
  auto& date_str = cached_date_str[seconds / UINT32_C(86400)];
  auto& time_str = cached_time_str[seconds % UINT32_C(86400)];
  if (date_str.empty() || time_str.empty()) {
    std::time_t time = seconds;
    struct tm tm;
    ::gmtime_r(&time, &tm);
    if (date_str.empty())
      StackWriter<14>().strftime("%F", &tm).finish(date_str);
    if (time_str.empty())
      StackWriter<14>().strftime("T%T.", &tm).finish(time_str);
  }
  BasicBufferWriter<sizeof(_result)> writer(_result);
  writer.printf(
      "%s%s%09" PRIu64,
      date_str.c_str(),
      time_str.c_str(),
      fraction).finish();
}

// BasicClockStr

BasicClockStr::BasicClockStr(std::chrono::nanoseconds nanoseconds) {
  auto count = nanoseconds.count();
  auto seconds = count / UINT64_C(1000000000);
  auto fraction = count % UINT64_C(1000000000);
  BasicBufferWriter<sizeof(_result)> writer(_result);
  writer.printf(
      "%" PRIu64 ".%09" PRIu64,
      seconds,
      fraction).finish();
}

}  // namespace stream
}  // namespace roq
