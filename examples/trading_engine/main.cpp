/* Copyright (c) 2017, Hans Erik Thrane */

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <quinclas/client/event_dispatcher.h>
#include <quinclas/client/request_dispatcher.h>

#include "your/strategy.h"

DEFINE_string(local_address, "", "local address (unix domain socket)");

int main(int argc, char *argv[]) {
  // initialize logging library
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  // parse command-line options
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ShutDownCommandLineFlags();
  // validate command-line options
  if (FLAGS_local_address.empty())
    throw std::runtime_error("local_address is missing");
  // we're ready
  LOG(INFO) << "===== START =====";
  // create host-internal socket address
  quinclas::io::net::UnixAddress address(FLAGS_local_address.c_str());
  // create host-internal socket
  quinclas::io::net::Socket socket(PF_LOCAL, SOCK_STREAM, 0);
  socket.non_blocking(true);
  // create event base
  quinclas::io::libevent::Base base;
  // TODO(thraneh): create buffer-event inside the event-loop (and recreate upon lost connection)
  // create buffer-event
  quinclas::io::libevent::BufferEvent buffer_event(base, std::move(socket));
  // create request dispatcher
  quinclas::client::RequestDispatcher request_dispatcher(buffer_event);
  // create your strategy
  examples::your::Strategy strategy(request_dispatcher);
  // create event dispatcher
  quinclas::client::EventDispatcher event_dispatcher(strategy, base, buffer_event);
  // create timer-event
  quinclas::io::libevent::TimerEvent timer(event_dispatcher, base);
  struct timeval timeout{ .tv_sec = 1, .tv_usec = 0 };
  timer.add(&timeout);
  // connect buffer-event
  buffer_event.connect(address);
  // run event loop
  base.loop(EVLOOP_NO_EXIT_ON_EMPTY);
  // we're done
  LOG(INFO) << "===== STOP =====";
  return EXIT_SUCCESS;  // TODO(thraneh): event-loop should be able to stop with an error code
}
