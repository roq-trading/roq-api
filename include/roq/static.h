/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <cstddef>

namespace roq {

// constexpr string utilities

constexpr std::size_t static_strlen(const char *s) {
  size_t res = 0;
  for (auto iter = s; *iter != '\0'; ++iter, ++res) {
    // empty
  }
  return res;
}
constexpr const char *static_strrchr(const char *s, char c) {
  const char *res = s;
  for (auto iter = s; *iter != '\0'; ++iter)
    if (*iter == c)
      res = iter;
  return res;
}
constexpr const char *static_basename(const char *path) {
  auto res = static_strrchr(path, '/');
  while (*res == '/')
    ++res;
  return res;
}

// inspired by
// https://github.com/Jiwan/meta_crush_saga/blob/master/constexpr_string.hpp

template <size_t N>
class static_string final {
 public:
  constexpr explicit static_string(const char(&text)[N])
      : _data{},
        _size(N - 1) {
    for (auto i = 0; i < N; ++i)
      _data[i] = text[i];
  }
  constexpr static_string(const char *text, size_t size)
      : _data{},
        _size(size) {
    for (auto i = 0; i < N && i < size; ++i)
      _data[i] = text[i];
  }
  template <size_t M>
  constexpr static_string(const static_string<M>& rhs, size_t size)
      : _data{},
        _size(size) {
    for (auto i = 0; i < rhs.size(); ++i)
      _data[i] = rhs[i];
  }

  constexpr const char* data() const {
    return _data;
  }

  constexpr size_t size() const {
    return _size;
  }

  constexpr char& operator[](size_t n) {
    return _data[n];
  }

  constexpr const char& operator[](size_t n) const {
    return _data[n];
  }

  template <size_t M>
  constexpr auto append(const static_string<M>& rhs) {
    static_string<N + M> res(*this, size() + rhs.size());
    for (auto i = 0; i < rhs.size(); ++i)
      res[size() + i] = rhs[i];
    return res;
  }

  constexpr auto basename() const {
    auto index = 0;
    for (auto i = 0; i < size(); ++i)
      if (_data[i] == '/')
        index = i;
    while (index < size() && _data[index] == '/')
      ++index;
    auto size = _size - index;
    return static_string<N>(_data + index, size);
  }

 private:
  char _data[N];
  size_t _size;
};

// FIXME(thraneh): workaround to manage gcc build issues

template <size_t N>
class static_basename_string final {
 public:
  constexpr explicit static_basename_string(const char(&text)[N])
      : _data{},
        _size(N - 1) {
    auto index = 0;
    for (auto i = 0; i < N; ++i)
      if (text[i] == '/')
        index = i;
    while (index < N && text[index] == '/')
      ++index;
    _size -= index;
    for (auto i = 0; i < (_size + 1); ++i)
      _data[i] = text[index + i];  // will include null terminator
  }

  constexpr const char* data() const {
    return _data;
  }

  constexpr size_t size() const {
    return _size;
  }

  constexpr char& operator[](size_t n) {
    return _data[n];
  }

  constexpr const char& operator[](size_t n) const {
    return _data[n];
  }

  template <size_t M>
  constexpr auto append(const static_string<M>& rhs) {
    static_string<N + M> res(data(), size() + rhs.size());
    for (auto i = 0; i < rhs.size(); ++i)
      res[size() + i] = rhs[i];
    return res;
  }

 private:
  char _data[N];
  size_t _size;
};

}  // namespace roq
