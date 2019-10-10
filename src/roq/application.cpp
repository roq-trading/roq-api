/* Copyright (c) 2017-2019, Hans Erik Thrane */

#include "roq/application.h"

#include <gflags/gflags.h>

#include "roq/logging.h"

namespace roq {

namespace {
static int initialize_gflags(
    int argc,
    char ***argv,
    const char *description,
    const char *version) {
  gflags::SetUsageMessage(description);
  gflags::SetVersionString(version);
  gflags::ParseCommandLineFlags(&argc, argv, true);
  gflags::ShutDownCommandLineFlags();
  return argc;
}
}  // namespace

Application::Application(
    int argc,
    char **argv,
    const char *description,
    const char *version)
    : _argv(argv),
      _argc(initialize_gflags(argc, &_argv, description, version)) {
  Logger::initialize();
}

Application::~Application() {
}

int Application::run() {
  LOG(INFO)("===== START =====");
  auto res = main(_argc, _argv);
  LOG(INFO)("===== STOP =====");
  return res;
}

}  // namespace roq
