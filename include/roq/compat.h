/* Copyright (c) 2017-2020, Hans Erik Thrane */

#pragma once

#if defined(__GNUC__) || defined(__clang__)
#define ROQ_PUBLIC __attribute__((visibility("default")))
#define ROQ_HOT __attribute__((hot))
#define ROQ_PACKED __attribute__((__packed__))
#else
#define ROQ_PUBLIC
#define ROQ_HOT
#define ROQ_PACKED
#endif
