/* Copyright (c) 2017-2024, Hans Erik Thrane */

#include <catch2/catch_all.hpp>

#include "roq/utils/common.hpp"

using namespace roq;
using namespace std::literals;

TEST_CASE("request_status_compare_requests", "[request_status]") {
  CHECK(utils::compare_requests(RequestStatus::TIMEOUT, RequestStatus::ACCEPTED) == std::strong_ordering::less);
}
