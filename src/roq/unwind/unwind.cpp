/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include "roq/unwind/unwind.h"

#define UNW_LOCAL_ONLY
#include <libunwind.h>

#include <cxxabi.h>
#include <stdio.h>
#include <stdlib.h>

namespace roq {
namespace unwind {

namespace {
static char proc_name[1024];
constexpr int width = (2 * sizeof(void *)) + 2;
}  // namespace

void print_stacktrace(int signal, siginfo_t *info) {
#if defined(__linux__)
  psiginfo(info, nullptr);
#endif
  unw_context_t uc;
  if (unw_getcontext(&uc) != 0) {
    fprintf(stderr, "Unable to initialize libunwind context.\n");
    return;
  }
  unw_cursor_t cursor;
  if (unw_init_local(&cursor, &uc) < 0) {
    fprintf(stderr, "Unable to initialize libunwind cursor.\n");
    return;
  }
  int status;
  for (int index = 0;; ++index) {
    status = unw_step(&cursor);
    if (status == 0)  // done
      break;
    if (status < 0) {  // failure
      fprintf(stderr, "Unable to step libunwind cursor.\n");
      return;
    }
    unw_word_t ip = 0;
    if (unw_get_reg(&cursor, UNW_REG_IP, &ip) < 0) {
      fprintf(stderr, "Unable to get libunwind ip register.\n");
    }
    unw_word_t offp;
    status = unw_get_proc_name(
        &cursor,
        proc_name,
        sizeof(proc_name),
        &offp);
    const char *name = "<unknown>";
    char *demangled_name = nullptr;
    if (status < 0) {
      if (status != UNW_ENOINFO) {
        fprintf(stderr, "Unable to get libunwind proc_name.\n");
      }
    } else {
      name = proc_name;
      demangled_name = abi::__cxa_demangle(
          proc_name,
          nullptr,
          nullptr,
          &status);
      if (status == 0)
        name = demangled_name;
    }
#if defined(__linux__)
    fprintf(stderr, "[%2d] %#*lx %s\n", index, width, ip, name);
#elif defined(__APPLE__)
    fprintf(stderr, "[%2d] %#*llx %s\n", index, width, ip, name);
#endif
    if (demangled_name)
      free(demangled_name);
  }
}

}  // namespace unwind
}  // namespace roq
