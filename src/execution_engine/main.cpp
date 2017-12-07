/* Copyright (c) 2017, Hans Erik Thrane */

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <quinclas/utils/event_dispatcher.h>
#include <quinclas/utils/request_dispatcher.h>

#include "execution_engine/strategy.h"

DEFINE_string(local_address, "", "local address (unix domain socket)");

int main(int argc, char *argv[]) {
  int result = EXIT_FAILURE;
  try {
    google::InitGoogleLogging(argv[0]);
    google::InstallFailureSignalHandler();

    gflags::ParseCommandLineFlags(&argc, &argv, true);
    gflags::ShutDownCommandLineFlags();

    if (FLAGS_local_address.empty())
      throw std::runtime_error("local_address is missing");

    LOG(INFO) << "*** START ***";

    // local (unix domain) socket address
    struct sockaddr_un address = {};
    address.sun_family = AF_LOCAL;
    strncpy(address.sun_path, FLAGS_local_address.c_str(), sizeof(address.sun_path));
    address.sun_path[sizeof(address.sun_path) - 1] = '\0';
    // initialize libevent base
    quinclas::libevent::Base base;
    // create a socket and wrap it for use by libevent
    quinclas::net::Socket socket(PF_LOCAL, SOCK_DGRAM, 0);
    socket.non_blocking(true);
    quinclas::libevent::BufferEvent buffer_event(base, std::move(socket));
    // create strategy (including request dispatcher, event dispatcher and timer)
    quinclas::execution_engine::RequestDispatcher request_dispatcher(buffer_event);
    quinclas::execution_engine::Strategy strategy(request_dispatcher);
    quinclas::execution_engine::EventDispatcher event_dispatcher(strategy, base, buffer_event);
    quinclas::libevent::TimerEvent timer(strategy, base);
    struct timeval timeout{ .tv_sec = 1, .tv_usec = 0 };
    timer.add(&timeout);
    // connect the socket
    buffer_event.connect(reinterpret_cast<const struct sockaddr *>(&address), sizeof(address));
    // start the libevent loop
    base.loop(EVLOOP_NO_EXIT_ON_EMPTY);

    result = EXIT_SUCCESS;
  }
  catch (std::exception& e) {
    LOG(ERROR) << "Exception: " << e.what();
  }
  catch (...) {
    LOG(ERROR) << "Exception: <unknown>";
  }

  LOG(INFO) << "*** STOP *** (exit code: " << result << ")";

  return result;
}
