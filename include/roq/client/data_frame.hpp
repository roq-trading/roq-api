/* Copyright (c) 2017-2023, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <span>
#include <string_view>

namespace roq {
namespace client {

class DataFrame {
 public:
  class Handler {
   public:
    virtual void add_series(
        std::span<std::string_view> const &values, std::string_view const &name, bool index, bool category) = 0;
    virtual void add_series(
        std::span<uint32_t> const &values, std::string_view const &name, bool index, bool category) = 0;
    virtual void add_series(
        std::span<double> const &values, std::string_view const &name, bool index, bool category) = 0;
    virtual void add_series(
        std::span<std::chrono::nanoseconds> const &values, std::string_view const &name, bool index, bool category) = 0;
  };
};

}  // namespace client
}  // namespace roq
