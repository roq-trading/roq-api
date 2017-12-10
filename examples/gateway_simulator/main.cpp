/* Copyright (c) 2017, Hans Erik Thrane */

#include <gflags/gflags.h>
#include <glog/logging.h>

#include <quinclas/client/event_dispatcher.h>
#include <quinclas/client/request_dispatcher.h>

#include "your/strategy.h"

DEFINE_string(local_address, "", "local address (unix domain socket)");

namespace {
class MyListener : public quinclas::io::libevent::Listener::Handler {
 public:
   MyListener(quinclas::io::net::Socket&& socket, quinclas::io::libevent::Base& base) :
     _listener(*this, base, LEV_OPT_REUSEABLE, 5, std::move(socket)) {
   }
 private:
  void on_accept(quinclas::io::libevent::BufferEvent&& buffer) override {
  }
  quinclas::io::libevent::Listener _listener;
};
}  // namespace

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
    unlink(FLAGS_local_address.c_str());
    quinclas::io::net::UnixAddress address(FLAGS_local_address.c_str());
    // initialize libevent base
    quinclas::io::libevent::Base base;
    // create a socket and wrap it for use by libevent
    quinclas::io::net::Socket socket(PF_LOCAL, SOCK_DGRAM, 0);
    socket.non_blocking(true);
    socket.bind(address);
    // listener
    MyListener listener(std::move(socket), base);
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
