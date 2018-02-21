/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <gflags/gflags.h>

#include <quinclas/client.h>
#include <quinclas/logging.h>

#include "reference/config_reader.h"
#include "reference/gateway_manager.h"

using namespace examples::reference;  // NOLINT

DEFINE_string(local_address, "",
    "Host-internal (aka unix domain) socket address (path)");

DEFINE_string(config_file, "",
    "Config filename for strategy (path)");

const char *GATEWAY = "FEMAS";

int main(int argc, char *argv[]) {
  // Initialize logging framework (default is: spdlog).
  quinclas::logging::Logger::initialize(true);

  // Parse command-line options.
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ShutDownCommandLineFlags();

  // Validate command-line options.
  if (FLAGS_local_address.empty()) {
    LOG(ERROR) << "Missing parameter: --local-address";
    return EXIT_FAILURE;
  }
  if (FLAGS_config_file.empty()) {
    LOG(ERROR) << "Missing parameter: --config-file";
    return EXIT_FAILURE;
  }

  // Create the gateway connection list.
  // TODO(thraneh): Need command-line format for fully specifying gateways.
  const std::unordered_map<std::string, std::string> gateways = {
    { GATEWAY, FLAGS_local_address },
  };

  try {
    // Parse config file.
    auto config = ConfigReader(FLAGS_config_file).parse();

    // Log config.
    LOG(INFO) << config;

    // Ready.
    LOG(INFO) << "===== START =====";

    // Atomically create Controller, Dispatcher and Strategy.
    // Then start the event loop.
    quinclas::client::Controller<GatewayManager>(
        std::move(gateways)).create_and_dispatch(std::move(config));

    // Done.
    LOG(INFO) << "===== STOP =====";

    return EXIT_SUCCESS;
  } catch (std::exception& e) {
    LOG(FATAL) << e.what();
  }
}
