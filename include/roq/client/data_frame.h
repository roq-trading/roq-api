/* Copyright (c) 2017-2022, Hans Erik Thrane */

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
        const std::span<std::string_view> &values, const std::string_view &name, bool index, bool category) = 0;
    virtual void add_series(
        const std::span<uint32_t> &values, const std::string_view &name, bool index, bool category) = 0;
    virtual void add_series(
        const std::span<double> &values, const std::string_view &name, bool index, bool category) = 0;
    virtual void add_series(
        const std::span<std::chrono::nanoseconds> &values, const std::string_view &name, bool index, bool category) = 0;
  };
};

}  // namespace client
}  // namespace roq
