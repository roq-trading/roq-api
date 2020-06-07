/* Copyright (c) 2017-2020, Hans Erik Thrane */

#include "roq/application.h"

#include <gflags/gflags.h>

#include <cassert>

#include "roq/logging.h"

#include "roq/builtins.h"

namespace roq {

namespace {
static int initialize_gflags(
    int argc,
    char ***argv,
    const std::string_view& description,
    const std::string_view& version) {
  assert(description.length() > 0);
  assert(version.length() > 0);
  gflags::SetUsageMessage(description.data());
  gflags::SetVersionString(version.data());
  gflags::ParseCommandLineFlags(&argc, argv, true);
  gflags::ShutDownCommandLineFlags();
  return argc;
}
}  // namespace

Application::Application(
    int argc,
    char **argv,
    const std::string_view& description,
    const std::string_view& version)
    : _argv(argv),
      _argc(initialize_gflags(argc, &_argv, description, version)) {
  Logger::initialize();
}

Application::~Application() {
}

int Application::run() {
  LOG(INFO)("===== START =====");
  auto res = main(_argc, _argv);
  LOG_IF(WARNING, res != 0)(
      FMT_STRING(R"(exit-code={})"),
      res);
  LOG(INFO)("===== STOP =====");
  return res;
}

}  // namespace roq
