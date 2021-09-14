/* Copyright (c) 2017-2021, Hans Erik Thrane */

#include <gtest/gtest.h>

#include "roq/utils/common.h"

using namespace roq;
using namespace roq::literals;

TEST(request_status, compare_requests) {
  EXPECT_EQ(utils::compare_requests(RequestStatus::TIMEOUT, RequestStatus::ACCEPTED), -1);
}
