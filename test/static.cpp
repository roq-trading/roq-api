/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include <gtest/gtest.h>

#include <stdexcept>

#include "roq/static.h"

#include "roq/logging.h"

using namespace roq;  // NOLINT

namespace {
/*
// SO
template <char... symbols>
struct String
{
    static constexpr char value[] = {symbols...};
};

template <char... symbols>
constexpr char String<symbols...>::value[];

template <typename, typename>
struct Concat;

template <char... symbols1, char... symbols2>
struct Concat<String<symbols1...>, String<symbols2...>>
{
    using type = String<symbols1..., symbols2...>;
};

template <typename...>
struct Concatenate;

template <typename S, typename... Strings>
struct Concatenate<S, Strings...>
{
    using type = typename Concat<S, typename Concatenate<Strings...>::type>::type;
};

template <>
struct Concatenate<>
{
    using type = String<>;
};

template <std::size_t N>
struct NumberToString
{
    using type = typename Concat
        <
            typename std::conditional<(N >= 10), typename NumberToString<N / 10>::type, String<>>::type,
            String<'0' + N % 10>
        >::type;
};

template <>
struct NumberToString<0>
{
    using type = String<'0'>;
};


template <const char *S>
struct MakeString {
    using type = typename Concat
        <
            typename std::conditional<(*S != '\0'), typename MakeString<S + 1>::type, String<>>::type,
            String<*S>
        >::type;
};

using xxx = MakeString<__FILE__>;

using Filename = String<'a', 'b', 'c'>;
constexpr const char *filename = Filename::value;

constexpr const std::size_t line = __LINE__;

using VersionString = Concatenate
    <
    String<
        NumberToString<line>::type,
        String<'.'>
    >::type;

constexpr const char* version_string = VersionString::value;
*/
/*
template <size_t N>
class static_string final {
 public:
  constexpr static_string(const char(&text)[N])
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
    if (n < _size)
      return _data[n];
    throw std::out_of_range("operator[]");
  }

  constexpr const char& operator[](size_t n) const {
    if (n < _size)
      return _data[n];
    throw std::out_of_range("operator[]");
  }

  template <size_t M>
  constexpr auto append(const static_string<M>& rhs) {
    static_string<N + M> res(*this, size() + rhs.size());
    for (auto i = 0; i < rhs.size(); ++i)
      res[size() + i] = rhs[i];
    return res;
  }

 private:
  char _data[N];
  size_t _size;
};
*/
/*
constexpr const char *static_concat(const char *lhs, const char *rhs) {
  return static_string(lhs).append(static_string(rhs)).data();
}
*/
/*
constexpr auto foo(const char *file) {
  constexpr auto basename = static_basename(file);
  constexpr auto size = static_strlen(basename) + 1;
  return static_string<size>(basename, size).append(
      static_string(":" STRINGIFY(__LINE__) "] "));
}
constepxr auto file = __FILE__;
constexpr auto xxx = foo(file);
*/
/*
constexpr auto xxx = static_string(__FILE__).append(
      static_string(":" STRINGIFY(__LINE__) "] "));
      */
static_assert(static_string("abc").basename().size() == 3);
static_assert(static_string("def/abc").basename().size() == 3);
static_assert(static_string("def//abc").basename().size() == 3);
/*
constexpr auto xxx = static_string(__FILE__).basename().append(
      static_string(":" STRINGIFY(__LINE__) "] "));
static_assert(static_strlen(xxx.data()) > 0);
*/
}  // namespace

TEST(static, simple) {
  static_assert(static_strlen("ABC") == 3);
  static_assert(static_strlen(static_strrchr("def", '/')) == 3);
  static_assert(static_strlen(static_strrchr("abc/def/ghi", '/')) == 4);
  static_assert(static_strlen(static_basename("abc/def/ghi")) == 3);
  auto bn = static_basename("abc/def/ghi");
  EXPECT_STREQ(bn, "ghi");
  auto fn = static_basename(__FILE__);
  EXPECT_STREQ(fn, "static.cpp");
  // EXPECT_STREQ(filename, "test_static.cpp");
  constexpr auto abcdef = static_string("abc").append(static_string("def"));
  static_assert(abcdef.size() == 6);
  static_assert(static_strlen(abcdef.data()) == 6);
  static_assert(static_strlen(static_string("abc").append(static_string("def")).data()) == 6);
  EXPECT_STREQ(abcdef.data(), "abcdef");
  static_string("abc").append(static_string(__FILE__));

  static_assert(static_basename_string("abc/def").size() == 3);
  static_assert(static_strlen(static_basename_string("abc/def").data()) == 3);
}
