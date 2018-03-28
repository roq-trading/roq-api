/* Copyright (c) 2017-2018, Hans Erik Thrane */

#define QUINCLAS_GLOG

#include <gtest/gtest.h>
#include <fcntl.h>
#include <roq/logging.h>

using namespace roq;  // NOLINT

TEST(logging_glog, simple) {
  logging::Logger::initialize();
  LOG(INFO) << "test";
  LOG(WARNING) << "test";
  LOG(ERROR) << "test";
  // FIXME(thraneh): LOG(FATAL) requires a SIGABRT handler...
  LOG_IF(INFO, 1 < 2) << "one is less than two";
  LOG_IF(INFO, 2 < 1) << "this can't be true!";
  open("/abc/def/ghi", O_RDONLY);
  PLOG(INFO) << "an expected error";
  logging::Logger::shutdown();
}
