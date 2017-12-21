/* Copyright (c) 2017, Hans Erik Thrane */

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <quinclas/client.h>

#include "trading_engine/strategy.h"

DEFINE_string(local_address, "", "host-internal socket address (path)");

using namespace examples::trading_engine;  // NOLINT

int main(int argc, char *argv[]) {
  // initialize logging library

  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  // parse command-line options

  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ShutDownCommandLineFlags();

  // validate command-line options

  if (FLAGS_local_address.empty()) {
    LOG(ERROR) << "local-address is a required parameter";
    return EXIT_FAILURE;
  }

  LOG(INFO) << "===== START =====";

  // configuration

  const uint32_t ticks_to_trade = 10;

  // create framework, instantiate strategy and start event dispatching

  quinclas::client::Controller<Strategy>({
      { "FEMAS", FLAGS_local_address },
    }).create_and_dispatch(ticks_to_trade);

  LOG(INFO) << "===== STOP =====";

  return EXIT_SUCCESS;
}
