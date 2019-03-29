/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/logging.h"

using namespace roq;  // NOLINT

TEST(logging, start_stop) {
  Logger::initialize();
  Logger::shutdown();
}
