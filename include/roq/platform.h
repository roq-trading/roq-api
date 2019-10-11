/* Copyright (c) 2017-2019, Hans Erik Thrane */

#pragma once

#include "roq/api.h"

namespace roq {

constexpr size_t cache_line_size() {
  return 64;
}

constexpr size_t page_size() {
  return 4096;
}

ROQ_PUBLIC size_t get_processor_count();
ROQ_PUBLIC void set_thread_affinity(size_t processor_index);

}  // namespace roq
