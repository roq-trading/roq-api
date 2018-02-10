/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <gflags/gflags.h>

#include <quinclas/client.h>
#include <quinclas/logging.h>

#include "collector/collector.h"

DEFINE_string(local_address, "", "host-internal socket address (path)");

using namespace examples::collector;  // NOLINT

int main(int argc, char *argv[]) {
  // initialize logging library
  quinclas::logging::Logger::initialize();

  // parse command-line options

  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ShutDownCommandLineFlags();

  // validate command-line options

  if (FLAGS_local_address.empty()) {
    LOG(ERROR) << "local-address is a required parameter";
    return EXIT_FAILURE;
  }

  LOG(INFO) << "===== START =====";

  // create framework, instantiate strategy and start event dispatching

  quinclas::client::Controller<Collector>({
      { "FEMAS", FLAGS_local_address },
    }).create_and_dispatch();

  LOG(INFO) << "===== STOP =====";

  return EXIT_SUCCESS;
}
