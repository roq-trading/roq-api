/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include "roq/options.h"

#include <gflags/gflags.h>

#include <cinttypes>

namespace {
static bool ValidatePositive(const char *flagname, int32_t value) {
  if (value >= 0)
    return true;
  printf("Invalid value for --%s: %" PRId32 "\n", flagname, value);
  return false;
}
}  // namespace

DEFINE_int32(socket_buffer_size, 0, "socket_buffer_size");
DEFINE_validator(socket_buffer_size, &ValidatePositive);

DEFINE_int32(busy_poll_usecs, 0, "busy_poll_usecs");
DEFINE_validator(busy_poll_usecs, &ValidatePositive);

namespace roq {

uint32_t Options::get_socket_buffer_size() {
  return static_cast<uint32_t>(FLAGS_socket_buffer_size);
}

uint32_t Options::get_busy_poll_usecs() {
  return static_cast<uint32_t>(FLAGS_busy_poll_usecs);
}

}  // namespace roq
