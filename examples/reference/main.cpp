/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <quinclas/client.h>

#include "reference/config_reader.h"
#include "reference/gateway_manager.h"

using namespace examples::reference;  // NOLINT

DEFINE_string(local_address, "",
    "Host-internal (aka unix domain) socket address (path)");

DEFINE_string(config_file, "",
    "Config filename for strategy (path)");

const char *GATEWAY = "FEMAS";

int main(int argc, char *argv[]) {
  // Initialize glog.
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  // Initialize gflags.
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ShutDownCommandLineFlags();

  // Validate command-line.
  if (FLAGS_local_address.empty()) {
    LOG(ERROR) << "Missing parameter: --local-address";
    return EXIT_FAILURE;
  }
  if (FLAGS_config_file.empty()) {
    LOG(ERROR) << "Missing parameter: --config-file";
    return EXIT_FAILURE;
  }

  // Create the gateway connection list.
  // TODO(thraneh): Invent a command-line format for fully specifying gateways.
  const std::unordered_map<std::string, std::string> gateways = {
    { GATEWAY, FLAGS_local_address },
  };

  try {
    // Read the strategy config file.
    auto config = ConfigReader(FLAGS_config_file).parse();

    LOG(INFO) << config;

    // Ready.
    LOG(INFO) << "===== START =====";

    // Create Controller, Dispatcher, Strategy, and then start the event loop.
    quinclas::client::Controller<GatewayManager>(
        std::move(gateways)).create_and_dispatch(std::move(config));

    // Done.
    LOG(INFO) << "===== STOP =====";

    return EXIT_SUCCESS;
  } catch (libconfig::ParseException& exception) {
    LOG(FATAL) << exception.what() << "("
        "file=" << exception.getFile() << ", "
        "line=" << exception.getLine() <<
        ")";
  }
}
