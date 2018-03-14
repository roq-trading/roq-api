/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <gflags/gflags.h>

#include <quinclas/client.h>
#include <quinclas/logging.h>

#include "strategy/strategy.h"

// Command line options (using gflags, see comments below):

DEFINE_string(gateways, "",
    "List of gateway connection details. "
    "Comma separated. "
    "For example: \"femas=unix:/var/tmp/femasapi.sock\".");

// Convenience (and not liked by cpplint).
using namespace examples::strategy;  // NOLINT

int main(int argc, char *argv[]) {
  // Initialize the logging library.
  // You have different choices for the logging backend.
  // Selectable at compile-time by choosing one of
  //   QUINCLAS_GLOG, QUINCLAS_SPDLOG, QUINCLAS_STDLOG
  quinclas::logging::Logger::initialize(true);

  // Parse the command-line options.
  // Here we use google's gflags library.
  // However, feel free to configure your application however you want.
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ShutDownCommandLineFlags();

  // Validate the command-line options.
  if (FLAGS_gateways.empty()) {
    LOG(ERROR) << "gateways is a required parameter";
    return EXIT_FAILURE;
  }

  LOG(INFO) << "===== START =====";

  // Parse connection details for all required gateways.
  auto gateways = quinclas::client::Gateways::create(FLAGS_gateways);

  // The controller is a pre-defined framework for managing
  // - IPC and/or network connectivity
  // - The communication protocol (encoding / decoding of messages)
  // - Event dispatching (from a gateway to your strategy)
  // - Request dispatching (from your strategy to a gateway)
  // You must instantiate it with the list of gateways required by your
  // strategy implementation.
  quinclas::client::Controller<Strategy> controller(std::move(gateways));

  // Your strategy configuration.
  // There's nothing special with this "configuration" -- it is only meant to
  // show how we can pass any kind of parameter(s) to your strategy.
  // Feel free to implement any kind of config file parsing at this point.
  const uint32_t ticks_to_trade = 10;

  // At this point we have only created configuration and set up a framework
  // for handling event and request dispatching -- no connectivity has been
  // attempted and your strategy has not been instantiated.

  // Next step is therefore to instantiate your strategy.
  // The controller is responsible for creating a (single) instance of your
  // strategy (remember: the controller knows of your strategy through the
  // template argument).
  // The controller will now atomically create the event/request dispatcher
  // implementations and an instance of your strategy in a *safe* order.
  // Your strategy will be constructed with a valid request dispatcher
  // interface -- you will *not* need to check for validity (e.g. nullptr)
  // and you will be guaranteed to not receive any events before your strategy
  // is fully instantiated.
  // This design is chosen for efficiency (nullptr check, locks, etc.) and for
  // safety (your strategy can't send requests before the dispatcher is
  // created, and you will not see events before your strategy is ready).
  controller.create_and_dispatch(ticks_to_trade);

  LOG(INFO) << "===== STOP =====";

  return EXIT_SUCCESS;
}
