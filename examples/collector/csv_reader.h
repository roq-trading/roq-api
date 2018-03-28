/* Copyright (c) 2017-2018, Hans Erik Thrane */

#pragma once

#include <roq/file.h>
#include <roq/memory.h>

#include <chrono>  // NOLINT
#include <string>
#include <utility>
#include <vector>

namespace examples {
namespace collector {

// Note! This is *not* meant to be a generic CSV reader.

class CsvReader final {
 public:
  CsvReader(const std::string& path, size_t max_columns);
  bool fetch();
  size_t length() const { return _length; }
  int get_integer(size_t index, int fallback = 0) const;
  double get_number(size_t index, double fallback = 0.0) const;
  std::string get_string(size_t index) const;
  std::chrono::system_clock::time_point get_time_point(size_t index, const char *format) const;

 private:
  const std::pair<const char *, size_t>& get_raw(size_t index) const;

 private:
  CsvReader(CsvReader&) = delete;
  CsvReader& operator=(CsvReader&) = delete;

 private:
  roq::file::File _file;
  roq::memory::Map _memory_map;
  const char *_current;
  const char *_end;
  std::vector<std::pair<const char *, size_t> > _columns;
  size_t _length;
};

}  // namespace collector
}  // namespace examples
