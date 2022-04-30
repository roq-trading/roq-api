/* Copyright (c) 2017-2022, Hans Erik Thrane */

#pragma once

#if __cplusplus >= 202002L
#include <span>
#else
#include <nonstd/span.hpp>
namespace std {
using nonstd::span;
}
#endif
