/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

// objective: prepare for using <ranges> (since C++20)

// https://github.com/ericniebler/range-v3
#include <range/v3/view.hpp>

// namespace envelope so we can more easily manage and replace
// dependencies when making the transition to C++20

namespace roq {
namespace ranges {

using ::ranges::iota_view;

}  // namespace ranges
}  // namespace roq
