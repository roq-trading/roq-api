/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <string>

// avoid warnings when using gcc 7.x

#ifdef major
#undef major
#endif

#ifdef minor
#undef minor
#endif

namespace roq {
namespace utils {

class Version final {
 public:
  explicit Version(const std::string& version) {
    auto iter = parse(_major, version, 0);
    iter = parse(_minor, version, iter);
    parse(_release, version, iter);
  }
  uint8_t major() const { return _major; }
  uint8_t minor() const { return _minor; }
  uint16_t release() const { return _release; }

 private:
  template <typename V>
  std::string::size_type parse(
      V& result,
      const std::string& version,
      const std::string::size_type begin) {
    if (begin == std::string::npos)
      return begin;
    auto next = version.find('.', begin);
    result = std::atoi(&version.at(begin));
    return next;
  }

 private:
  uint8_t _major = 0;
  uint8_t _minor = 0;
  uint16_t _release = 0;
};

static inline char *print_0_99(int val_0_99, char *ptr) {
  // Andrei Alexandrescu (2013): "Three Optimization Tips for C++"
  static const char digits[201] =
      "0001020304050607080910111213141516171819"
      "2021222324252627282930313233343536373839"
      "4041424344454647484950515253545556575859"
      "6061626364656667686970717273747576777879"
      "8081828384858687888990919293949596979899";
  assert(val_0_99 >= 0 && val_0_99 < 100);
  auto index = 2 * val_0_99;
  *(--ptr) = digits[index + 1];
  *(--ptr) = digits[index];
  return ptr;
}

static inline char *print_delim(char val, char *ptr) {
  *(--ptr) = val;
  return ptr;
}

struct YMD final {
  int year;
  int month;
  int day;
};

struct JulianDayNumber final {
  static YMD to_calendar(int jdn) {
    // Richards (2013): found on Wikipedia
    auto f = jdn + 1401 + (((4 * jdn + 274277) / 146097) * 3) / 4 - 38;
    auto e = 4 * f + 3;
    auto g = (e % 1461) / 4;
    auto h = 5 * g + 2;
    YMD result = {};
    result.day = (h % 153) / 5 + 1;
    result.month = ((h / 153 + 2) % 12) + 1;
    result.year = (e / 1461) - 4716 + (12 + 2 - result.month) / 12;
    return result;
  }
  static int from_calendar(int year, int month, int day) {
    // Doggett (1992): found on Wikipedia
    return (1461 * (year + 4800 + (month - 14) / 12)) / 4 +
      (367 * (month - 2 - 12 * (( month - 14) / 12))) / 12 -  // NOLINT
      (3 * ((year + 4900 + (month - 14) / 12) / 100)) / 4 +
      day -
      32075;
  }
  static int from_calendar(YMD ymd) {
    return from_calendar(ymd.year, ymd.month, ymd.day);
  }
  static ssize_t printf(char *buffer, size_t length, int jdn) {
    assert(length >= 10);
    auto ptr = print_raw(jdn, buffer + 10);
    assert(ptr == buffer);
    return 10;
  }
  static inline char *print_raw(int jdn, char *ptr) {
    auto ymd = to_calendar(jdn);
    return print_0_99(
      ymd.year / 100,
      print_0_99(
        ymd.year % 100,
        print_delim(
          '-',
          print_0_99(
            ymd.month,
            print_delim(
              '-',
              print_0_99(
                ymd.day,
                ptr))))));
  }
};

// FIXME(thraneh): option -- print seconds or microseconds
struct DateTime final {
  // FIXME(thraneh): use chrono (now assuming microseconds)
  static ssize_t printf(char *buffer, size_t length, uint64_t time) {
    assert(length >= 26);
    auto microseconds = time % 1000000;
    time /= 1000000;
    auto seconds = time % 86400;
    // FIXME(thraneh): use chrono
    // static int epoch = JulianDayNumber::from_calendar(1970, 1, 1);
    // auto jdn = epoch + (time / 86400);
    auto jdn = (time / 86400);
    auto ptr = JulianDayNumber::print_raw(jdn,
      print_delim(
        ' ',
        print_seconds(
          seconds,
          print_delim(
            '.',
            print_fraction(
              microseconds,
              buffer + 26)))));
    assert(ptr == buffer);
    return 26;
  }
  static inline char *print_seconds(int time, char *ptr) {
    assert(time >= 0 && time < 86400);
    auto seconds = time % 60;
    auto tmp = time / 60;
    auto minutes = tmp % 60;
    auto hours = tmp / 60;
    return print_0_99(
      hours,
      print_delim(
        ':',
        print_0_99(
          minutes,
          print_delim(
            ':',
            print_0_99(
              seconds,
              ptr)))));
  }
  static inline char *print_fraction(int fraction, char *ptr) {
    assert(fraction >= 0 && fraction < 1000000);
    auto frac_0 = fraction % 100;
    fraction /= 100;
    auto frac_1 = fraction % 100;
    fraction /= 100;
    auto frac_2 = fraction % 100;
    return print_0_99(
      frac_2,
      print_0_99(
        frac_1,
        print_0_99(
          frac_0,
          ptr)));
  }
};

}  // namespace utils
}  // namespace roq
