/* Copyright (c) 2017, Hans Erik Thrane */

#include <gflags/gflags.h>
#include <glog/logging.h>

// currently in this directory  -- will be moved into quinclas/tradingapi when feature complete
#include "gateway_simulator/controller.h"

DEFINE_string(local_address, "", "host-internal socket address (path)");

namespace {
// communicates with a strategy
class Strategy : public examples::gateway_simulator::Client {
 public:
  Strategy(quinclas::io::libevent::BufferEvent&& buffer_event, Remove remove, const int latency)
      : _buffer_event(std::move(buffer_event)), _remove(remove), _latency(latency) {
    _buffer_event.setcb(on_read, nullptr, on_error, this);
    _buffer_event.enable(EV_READ);
  }

 private:
  static void on_read(struct bufferevent *bev, void *arg) {
  }
  static void on_error(struct bufferevent *bev, short what, void *arg) {  // NOLINT
    auto& self = *reinterpret_cast<Strategy *>(arg);
    self._remove(&self);  // note! we need this callback for the garbage collection
  }

 private:
  Remove _remove;
  quinclas::io::libevent::BufferEvent _buffer_event;
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

  const auto handler = [&](quinclas::io::libevent::BufferEvent&& buffer_event,
                           examples::gateway_simulator::Client::Remove remove) {
    return std::unique_ptr<examples::gateway_simulator::Client>(
        new Strategy(std::move(buffer_event), remove, latency));
  };

  examples::gateway_simulator::Controller({
    { FLAGS_local_address, handler },
  }).dispatch();

  LOG(INFO) << "===== STOP =====";

  return EXIT_SUCCESS;
}
