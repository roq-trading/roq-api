/* Copyright (c) 2017-2024, Hans Erik Thrane */

#pragma once

#include <fmt/format.h>

#include <algorithm>
#include <cassert>
#include <span>
#include <string_view>

#include "roq/exceptions.hpp"

namespace roq {
namespace utils {

//! Simple wrapper around a fixed length buffer
template <typename T>
struct Message final {
  using value_type = T;
  using reference = value_type &;
  using const_reference = value_type const &;
  using pointer = value_type *;
  using const_pointer = value_type const *;
  using iterator = pointer;
  using const_iterator = const_pointer;

  explicit constexpr Message(std::span<value_type> const &buffer)
      : end_(&*std::end(buffer)), begin_(&*std::begin(buffer)), iter_(begin_) {}

  template <typename U = value_type>
    requires(std::is_same<value_type, typename U::value_type>::value)
  explicit constexpr Message(U &buffer) : end_(&*std::end(buffer)), begin_(&*std::begin(buffer)), iter_(begin_) {}

  template <typename U>
    requires(!std::is_same<value_type, typename U::value_type>::value)
  explicit constexpr Message(U &buffer)
      : end_(reinterpret_cast<const_pointer>(&*std::end(buffer))),
        begin_(reinterpret_cast<pointer>(&*std::begin(buffer))), iter_(begin_) {}

  Message(Message const &) = delete;
  Message(Message &&) = delete;

  Message &operator=(Message const &) = default;
  Message &operator=(Message &&) = default;

  inline std::span<std::byte const> as_bytes() const { return {reinterpret_cast<std::byte const *>(data()), size()}; }

  // note! only enable for T==char ?
  inline operator std::string_view() const { return {data(), size()}; }

  inline constexpr pointer data() noexcept { return begin_; }
  inline constexpr const_pointer data() const noexcept { return begin_; }

  inline constexpr size_t capacity() const noexcept { return end_ - begin_; }

  inline constexpr size_t size() const noexcept { return static_cast<size_t>(iter_ - begin_); }

  inline constexpr bool empty() const noexcept { return size() == 0; }

  inline constexpr void resize(size_t length) {
    using namespace std::literals;
    if (length > static_cast<size_t>(end_ - begin_)) [[unlikely]]
      throw LengthError{"buffer length exceeded (length={}, size={}, capacity={})"sv, length, size(), capacity()};
    iter_ = begin_ + length;
  }

  inline constexpr reference operator[](size_t offset) noexcept { return begin_[offset]; }
  inline constexpr const_reference operator[](size_t offset) const noexcept { return begin_[offset]; }

  inline constexpr iterator begin() noexcept { return begin_; }
  inline constexpr const_iterator begin() const noexcept { return begin_; }

  inline constexpr iterator end() noexcept { return iter_; }
  inline constexpr const_iterator end() const noexcept { return iter_; }

  inline constexpr Message &append(const_iterator begin, const_iterator end) {
    auto length = end - begin;
    auto iter = iter_;
    resize(size() + length);
    std::copy(begin, end, iter);
    return *this;
  }

  inline constexpr Message &append(std::span<std::byte const> const &buffer) {
    return append(std::data(buffer), std::data(buffer) + std::size(buffer));
  }

  inline constexpr Message &append(value_type value) {
    push_back(value);
    return *this;
  }

  inline constexpr void push_back(value_type value) noexcept {
    assert(iter_ < end_);
    if (iter_ < end_) {
      *iter_ = value;
      advance();
    }
  }

 private:
  inline constexpr Message &advance() noexcept {
    assert(iter_ < end_);
    ++iter_;
    return *this;
  }

 private:
  const_pointer end_;
  pointer begin_;
  pointer iter_;
};

}  // namespace utils
}  // namespace roq

// note!
// fmt has specialized for std::basic_string (rather than testing for features)
// making it possible to use resize() and append() together with std::back_inserter
// the following adds a specialization for this class so it can use resize/append
// however, there are use-cases where this actually makes the code slower...
namespace fmt {
template <>
struct is_contiguous<roq::utils::Message<char>> : std::true_type {};
}  // namespace fmt
static_assert(fmt::is_contiguous<roq::utils::Message<char>>::value == true);
