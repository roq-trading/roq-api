/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <gflags/gflags.h>

#include <quinclas/client.h>
#include <quinclas/simulation.h>
#include <quinclas/logging.h>

#include "reference/config_reader.h"
#include "reference/gateway_manager.h"
#include "reference/generator.h"

using namespace examples::reference;  // NOLINT

DEFINE_string(config_file, "",
    "Config filename for strategy (path)");

DEFINE_string(mode, "",
    "Mode of operation (trading|simulation)");

DEFINE_string(gateways, "",
    "List of gateway connection details. "
    "Comma separated. "
    "For example: \"femas=user:password@:/var/tmp/femasapi.sock\".");

DEFINE_string(simulation_file, "",
    "Simulation file (path)");

const char *GATEWAY = "FEMAS";

int main(int argc, char *argv[]) {
  quinclas::logging::Logger::initialize(true);

  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ShutDownCommandLineFlags();

  if (FLAGS_config_file.empty()) {
    LOG(ERROR) << "Missing parameter: --config-file";
    std::exit(EXIT_FAILURE);
  }

  auto config = ConfigReader(FLAGS_config_file).parse();

  LOG(INFO) << config;

  if (FLAGS_mode == "trading") {
    if (FLAGS_gateways.empty()) {
      LOG(ERROR) << "Missing parameter: --gateways";
      std::exit(EXIT_FAILURE);
    }

    // Parse connection details for all required gateways.
    auto gateways = quinclas::client::Gateways::create(FLAGS_gateways);

    quinclas::client::Controller<GatewayManager>(
        std::move(gateways)).create_and_dispatch(std::move(config));

  } else if (FLAGS_mode == "simulation") {
    if (FLAGS_simulation_file.empty()) {
      LOG(ERROR) << "Missing parameter: --simulation-file";
      std::exit(EXIT_FAILURE);
    }

    std::list<std::unique_ptr<quinclas::simulation::Generator> > generators;
    generators.emplace_back(new Generator(FLAGS_simulation_file));

    quinclas::simulation::Controller<GatewayManager>(
        std::move(generators)).create_and_dispatch(std::move(config));

  } else {
    LOG(ERROR) << "Invalid parameter: mode=\"" << FLAGS_mode << "\"";
    std::exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
