/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#include <string_view>

namespace roq {
namespace metrics {

constexpr auto const COUNTER = []() {
  using namespace std::literals;
  return "roq_counter"sv;
}();

constexpr auto const INTER_PROCESS_LATENCY = []() {
  using namespace std::literals;
  return "roq_inter_process_latency"sv;
}();

constexpr auto const LATENCY = []() {
  using namespace std::literals;
  return "roq_latency"sv;
}();

constexpr auto const PROFILE = []() {
  using namespace std::literals;
  return "roq_profile"sv;
}();

}  // namespace metrics
}  // namespace roq
