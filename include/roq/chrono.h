/* Copyright (c) 2017-2021, Hans Erik Thrane */

#pragma once

#include <chrono>

#include "roq/compat.h"

namespace roq {
namespace chrono {

// https://stackoverflow.com/questions/42114518/stdchrono-add-days-to-current-date
using days = std::chrono::duration<
    int,
    std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;

}  // namespace chrono
}  // namespace roq
