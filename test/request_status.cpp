/* Copyright (c) 2017-2022, Hans Erik Thrane */

#include <catch2/catch.hpp>

#include "roq/utils/common.h"

using namespace roq;
using namespace std::literals;

TEST_CASE("request_status_compare_requests", "[request_status]") {
  CHECK(utils::compare_requests(RequestStatus::TIMEOUT, RequestStatus::ACCEPTED) == -1);
}
