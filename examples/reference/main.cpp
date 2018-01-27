/* Copyright (c) 2017-2018, Hans Erik Thrane */

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <quinclas/client.h>

#include "reference/config.h"
#include "reference/strategy.h"

DEFINE_string(config_file, "", "Config filename (path)");

using namespace examples::reference;  // NOLINT

int main(int argc, char *argv[]) {
  // Initialize glog.
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  // Initialize gflags.
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ShutDownCommandLineFlags();

  // Validate command-line.
  if (FLAGS_config_file.empty()) {
    LOG(ERROR) << "config-file is a required parameter";
    return EXIT_FAILURE;
  }

  try {
  // Read config file.
    Config config(FLAGS_config_file);

    // Ready.
    LOG(INFO) << "===== START =====";

    // Create and dispatch.
    quinclas::client::Controller<Strategy>(config.gateways).create_and_dispatch(config);

    // Done.
    LOG(INFO) << "===== STOP =====";

    return EXIT_SUCCESS;
  } catch (libconfig::ParseException& exception) {
    LOG(FATAL) << exception.what() << "(file=" << exception.getFile() << ", line=" << exception.getLine() << ")";
  }
}
