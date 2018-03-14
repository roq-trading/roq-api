/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <gflags/gflags.h>

#include <quinclas/client.h>
#include <quinclas/logging.h>

#include "collector/collector.h"

DEFINE_string(gateways, "",
    "List of gateway connection details. "
    "Comma separated. "
    "For example: \"femas=unix:/var/tmp/femasapi.sock\".");

using namespace examples::collector;  // NOLINT

int main(int argc, char *argv[]) {
  // initialize logging library
  quinclas::logging::Logger::initialize(true);

  // parse command-line options

  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ShutDownCommandLineFlags();

  // validate command-line options

  if (FLAGS_gateways.empty()) {
    LOG(ERROR) << "gateways is a required parameter";
    return EXIT_FAILURE;
  }

  // parse connection details for all required gateways

  auto gateways = quinclas::client::Gateways::create(FLAGS_gateways);

  LOG(INFO) << "===== START =====";

  // create framework, instantiate strategy and start event dispatching

  quinclas::client::Controller<Collector>(
      std::move(gateways)).create_and_dispatch();

  LOG(INFO) << "===== STOP =====";

  return EXIT_SUCCESS;
}
