/* Copyright (c) 2017-2018, Hans Erik Thrane */

#define QUINCLAS_SPDLOG

#include <gtest/gtest.h>
#include "quinclas/logging.h"
#include <fcntl.h>

using namespace quinclas;  // NOLINT

TEST(logging_spdlog, simple) {
  logging::Logger(0, nullptr);
  LOG(INFO) << "test";
  LOG(WARNING) << "test";
  LOG(ERROR) << "test";
  // FIXME(thraneh): LOG(FATAL) requires a SIGABRT handler...
  LOG_IF(INFO, 1<2) << "one is less than two";
  LOG_IF(INFO, 2<1) << "this can't be true!";
  open("/abc/def/ghi", O_RDONLY);
  PLOG(INFO) << "an expected error";
}
