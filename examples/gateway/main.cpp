/* Copyright (c) 2017, Hans Erik Thrane */

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <quinclas/server.h>

#include "gateway/strategy.h"

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

  // this is a good place to spawn a thread if market access is through an API
  // --> how does it find the connected clients ??? (do we need a queue?)

  // configuration

  const uint32_t latency = 10;

  // handler

  const auto handler = [latency](quinclas::server::Connection::Dispatcher& dispatcher) {
    return std::unique_ptr<quinclas::server::Connection>(
        new Strategy(dispatcher, latency));
  };

  quinclas::server::Controller({
    { FLAGS_local_address, handler },
  }).dispatch();

  LOG(INFO) << "===== STOP =====";

  return EXIT_SUCCESS;
}
