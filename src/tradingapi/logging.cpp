/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include "quinclas/logging.h"
#include <iostream>
// FIXME(thraneh): only do this when configure has detected spdlog
#include <spdlog/spdlog.h>

namespace quinclas {
namespace logging {
namespace detail {
thread_local char message_buffer_raw[MESSAGE_BUFFER_SIZE];
thread_local char *message_buffer = message_buffer_raw;
spdlog::logger *spdlog_logger = nullptr;
}  // namespace detail
}  // namespace logging
}  // namespace quinclas
