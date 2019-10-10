/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include "roq/core/affinity.h"

#if defined(__linux__)
#include <sys/sysinfo.h>
#endif

#if defined(__APPLE__)
#include <sys/types.h>
#include <sys/sysctl.h>
#endif

#if !defined(_GNU_SOURCE)
#define _GNU_SOURCE
#endif
#include <sched.h>

#include <system_error>

#include "roq/logging.h"

namespace roq {
namespace ipc {

size_t get_processor_count() {
#if defined(__APPLE__)
  int32_t result;
  size_t length = sizeof(result);
  if (::sysctlbyname(
        "hw.logicalcpu_max",
        &result,
        &length,
        nullptr,
        0) != 0)
    throw std::system_error(errno, std::system_category());
  return result;
#else
  return ::get_nprocs();
#endif
}

void set_thread_affinity(size_t processor_index) {
#if defined(__APPLE__)
  // no support
#else
  cpu_set_t cpu_set;
  CPU_ZERO_S(sizeof(cpu_set), &cpu_set);
  CPU_SET_S(processor_index, sizeof(cpu_set), &cpu_set);
  if (::pthread_setaffinity_np(
        pthread_self(),
        sizeof(cpu_set),
        &cpu_set) != 0) {
    PLOG(WARNING)("pthread_setaffinity_np() failed");
    throw std::system_error(errno, std::system_category());
  }
#endif
}

}  // namespace ipc
}  // namespace roq
