/* Copyright (c) 2017-2025, Hans Erik Thrane */

#pragma once

#if defined(__clang__)
#if __clang_major__ >= 21
#include <algorithm>  // used by fmt/color.h without including
#include <cstdlib>    // malloc/free used by fmt/format.h without including (fmt 11.2 + clang 21.x)
#endif
#endif
