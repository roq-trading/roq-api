/* Copyright (c) 2017, Hans Erik Thrane */

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <quinclas/server.h>

#include "gateway/gateway.h"

using namespace examples::gateway;  // NOLINT

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

  // configuration

  const uint32_t latency = 10;

  // create framework, instantiate gateway and start even dispatching

  quinclas::server::Controller<Gateway>({
    { FLAGS_local_address, },
  }).create_and_dispatch(latency);

  LOG(INFO) << "===== STOP =====";

  return EXIT_SUCCESS;
}
