/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#include <string_view>

namespace roq {
namespace metrics {

constexpr std::string_view COUNTER("roq_counter");
constexpr std::string_view INTER_PROCESS_LATENCY("roq_inter_process_latency");
constexpr std::string_view LATENCY("roq_latency");
constexpr std::string_view PROFILE("roq_profile");

}  // namespace metrics
}  // namespace roq
