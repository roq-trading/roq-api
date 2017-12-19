/* Copyright (c) 2017, Hans Erik Thrane */

#include <gflags/gflags.h>
#include <glog/logging.h>

// currently in this directory  -- will be moved into quinclas/tradingapi when feature complete
#include "gateway_simulator/controller.h"
// #include <quinclas/server/controller.h>

DEFINE_string(local_address, "", "host-internal socket address (path)");

namespace {
// communicates with a strategy
class Strategy : public examples::gateway_simulator::Client {
 public:
  Strategy(examples::gateway_simulator::Client::Writer& writer, const int latency)
      : _writer(writer), _latency(latency) {}

 private:
  void on_read() override {
  }

 private:
  examples::gateway_simulator::Client::Writer& _writer;
  int _latency;
};
}  // namespace

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

  // handler

  const auto handler = [&](examples::gateway_simulator::Client::Writer& writer) {
    return std::unique_ptr<examples::gateway_simulator::Client>(new Strategy(writer, latency));
  };

  examples::gateway_simulator::Controller({
    { FLAGS_local_address, handler },
  }).dispatch();

  LOG(INFO) << "===== STOP =====";

  return EXIT_SUCCESS;
}
