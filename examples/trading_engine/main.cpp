/* Copyright (c) 2017, Hans Erik Thrane */

#include <gflags/gflags.h>
#include <glog/logging.h>

// currently in this directory  -- will be moved into quinclas/tradingapi when feature complete
#include "trading_engine/controller.h"

#include "your/strategy.h"

DEFINE_string(local_address, "", "host-internal socket address (path)");

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

  // create framework, instantiate strategy and start event dispatching

  examples::trading_engine::Controller<examples::your::Strategy>({
      { "FEMAS", FLAGS_local_address },
    }).create_and_dispatch(
      /*ticks_to_trade =*/ 10);

  LOG(INFO) << "===== STOP =====";

  return EXIT_SUCCESS;
}
