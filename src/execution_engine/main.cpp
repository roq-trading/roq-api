/* Copyright (c) 2017, Hans Erik Thrane */

#include <glog/logging.h>
#include <gflags/gflags.h>

#include "execution_engine/dispatcher.h"
#include "execution_engine/strategy.h"
#include "execution_engine/controller.h"

DEFINE_string(local_address, "", "local address (unix domain socket)");

namespace {
class Address {
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

        struct sockaddr_un address = {};
        address.sun_family = AF_LOCAL;
        strncpy(address.sun_path, FLAGS_local_address.c_str(), sizeof(address.sun_path));
        address.sun_path[sizeof(address.sun_path) - 1] = '\0';

        quinclas::event::Base base;
        quinclas::net::Socket socket(PF_LOCAL, SOCK_DGRAM, 0);
        socket.non_blocking(true);
        quinclas::event::BufferEvent buffer_event(base, std::move(socket));
        quinclas::execution_engine::Dispatcher dispatcher(buffer_event);
        quinclas::execution_engine::Strategy strategy(dispatcher);
        quinclas::execution_engine::Controller controller(base, buffer_event, strategy);
        buffer_event.connect(reinterpret_cast<const struct sockaddr *>(&address), sizeof(address));
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
