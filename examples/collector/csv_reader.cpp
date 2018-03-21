/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "collector/csv_reader.h"

#include <cstring>
#include <iomanip>
#include <sstream>

#define DELIM ','

namespace {
static const char *EMPTY = "";
}  // namespace

namespace examples {
namespace collector {

CsvReader::CsvReader(const std::string& path, size_t max_columns)
    : _file(path, O_RDONLY),
      _memory_map(_file.size(), PROT_READ, MAP_SHARED, _file),
      _current(reinterpret_cast<const char *>(_memory_map.get())),
      _end(_current + _memory_map.length()),
      _columns(max_columns),
      _length(0) {}

bool CsvReader::fetch() {
  auto eol = reinterpret_cast<const char *>(std::memchr(_current, '\n', _end - _current));
  if (eol == nullptr)
    eol = _end;
  auto end = (eol > _current && *(eol - 1) == '\r') ? eol - 1 : eol;
  auto iter = _current;
  for (_length = 0; _length < _columns.size() && iter < end; ++_length) {
    if ((*iter) == DELIM) {
      _columns[_length] = std::make_pair(EMPTY, 1);
      ++iter;
    } else {
      auto bytes = eol - iter;
      auto delim = reinterpret_cast<const char *>(std::memchr(iter, DELIM, bytes));
      if (delim == nullptr)
        delim = eol;
      _columns[_length] = std::make_pair(iter, delim - iter);
      iter = delim + 1;
    }
  }
  _current = eol < _end ? eol + 1 : _end;
  return _length > 0;
}

int CsvReader::get_integer(size_t index, int fallback) const {
  const auto& value = get_raw(index);
  if (value.second == 0)
    return 0;
  std::string tmp(value.first, value.second);
  return atoi(tmp.c_str());
}

double CsvReader::get_number(size_t index, double fallback) const {
  const auto& value = get_raw(index);
  if (value.second == 0)
    return 0.0;
  std::string tmp(value.first, value.second);
  if (tmp == "NaN")
    return 0.0;
  return atof(tmp.c_str());
}

std::string CsvReader::get_string(size_t index) const {
  const auto& value = get_raw(index);
  return value.second ? std::string(value.first, value.second) : std::string();
}

std::chrono::system_clock::time_point CsvReader::get_time_point(size_t index, const char *format) const {
  const auto& value = get_raw(index);
  std::tm tm = {};
  std::stringstream ss(std::string(value.first, value.second));
  ss >> std::get_time(&tm, format);
  auto result = std::chrono::system_clock::from_time_t(std::mktime(&tm));
  auto separator = reinterpret_cast<const char *>(std::memchr(value.first, '.', value.second));
  if (separator == nullptr)
    return result;
  auto fraction = separator + 1;
  switch (value.second - (fraction - value.first)) {
    case 3:
      return result + std::chrono::milliseconds(atoi(fraction));
    case 6:
      return result + std::chrono::microseconds(atoi(fraction));
    case 9:
      // std::chrono::nanoseconds doesn't seemt to be available when using clang 9.0.0 on darwin
      return result + std::chrono::microseconds(atoi(fraction) / 1000);
    default:
      throw std::runtime_error("Unknow time fraction");
  }
}

const std::pair<const char *, size_t>& CsvReader::get_raw(size_t index) const {
  if (index < length())
    return _columns[index];
  throw std::out_of_range("Not a valid column");
}

}  // namespace collector
}  // namespace examples
