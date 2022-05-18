/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <chrono>
#include <string_view>

#include "roq/span.hpp"

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
