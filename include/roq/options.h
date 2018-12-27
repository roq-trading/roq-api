/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include <cstdint>

namespace roq {

struct Options final {
  static uint32_t get_socket_buffer_size();
  static uint32_t get_busy_poll_usecs();
};

}  // namespace roq
