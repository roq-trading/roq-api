/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <quinclas/server.h>

#include "gateway/gateway.h"

using namespace examples::gateway;  // NOLINT

DEFINE_string(local_address, "", "host-internal socket address (path)");
DEFINE_int32(monitor_port, 0, "monitor port (port)");

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

  if (FLAGS_monitor_port == 0) {
    LOG(ERROR) << "monitor-port is a required parameter";
    return EXIT_FAILURE;
  } else if (FLAGS_monitor_port < 0 || FLAGS_monitor_port > std::numeric_limits<uint16_t>::max()) {
    LOG(ERROR) << "monitor-port is out of range";
    return EXIT_FAILURE;
  }

  LOG(INFO) << "===== START =====";

  // configuration

  const uint32_t latency = 10;

  // create framework, instantiate gateway and start even dispatching

  quinclas::server::Controller<Gateway>(
      static_cast<uint16_t>(FLAGS_monitor_port),
      { FLAGS_local_address }).create_and_dispatch(latency);

  LOG(INFO) << "===== STOP =====";

  return EXIT_SUCCESS;
}
