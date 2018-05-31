/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <string>

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

}  // namespace utils
}  // namespace roq
