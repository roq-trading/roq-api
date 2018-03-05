/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <gflags/gflags.h>

#include <quinclas/client.h>
#include <quinclas/simulation.h>
#include <quinclas/logging.h>

#include "reference/config_reader.h"
#include "reference/gateway_manager.h"
#include "reference/generator.h"

using namespace examples::reference;  // NOLINT

// common

DEFINE_string(mode, "trading",
    "Mode of operation (trading|simulation)");

DEFINE_string(config_file, "",
    "Config filename for strategy (path)");

// mode: trading

DEFINE_string(local_address, "",
    "Host-internal (aka unix domain) socket address (path)");

// mode: simulation

DEFINE_string(simulation_file, "",
    "Simulation file (path)");

const char *GATEWAY = "FEMAS";

int main(int argc, char *argv[]) {
  // Initialize logging framework
  quinclas::logging::Logger::initialize(true);

  // Parse command-line options.
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ShutDownCommandLineFlags();

  // Config file
  if (FLAGS_config_file.empty()) {
    LOG(ERROR) << "Missing parameter: --config-file";
    std::exit(EXIT_FAILURE);
  }
  auto config = ConfigReader(FLAGS_config_file).parse();
  LOG(INFO) << config;

  if (FLAGS_mode == "trading") {  // trading
    // Gateway connections
    if (FLAGS_local_address.empty()) {
      LOG(ERROR) << "Missing parameter: --local-address";
      std::exit(EXIT_FAILURE);
    }
    std::unordered_map<std::string, std::string> gateways;
    gateways.emplace(GATEWAY, FLAGS_local_address);
    // Create trading engine, strategy, then dispatch
    quinclas::client::Controller<GatewayManager>(
        std::move(gateways)).create_and_dispatch(std::move(config));
  } else if (FLAGS_mode == "simulation") {  // simulation
    // Simulation files
    if (FLAGS_simulation_file.empty()) {
      LOG(ERROR) << "Missing parameter: --simulation-file";
      std::exit(EXIT_FAILURE);
    }
    std::list<std::unique_ptr<quinclas::simulation::Generator> > generators;
    generators.emplace_back(new Generator(FLAGS_simulation_file));
    // Create simulation engine, strategy, then dispatch
    quinclas::simulation::Controller<GatewayManager>(
        std::move(generators)).create_and_dispatch(std::move(config));
  } else {  // invalid
    LOG(ERROR) << "Invalid parameter: mode=\"" << FLAGS_mode << "\"";
    std::exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
