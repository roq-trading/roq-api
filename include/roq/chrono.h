/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <chrono>

#include "roq/compat.h"

// objective: preparing for c++20

namespace roq {
namespace chrono {

// references:
//   https://stackoverflow.com/a/42116047
using days = std::chrono::duration<int, std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;

}  // namespace chrono
}  // namespace roq
